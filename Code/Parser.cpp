#include "Parser.hpp"
#include <algorithm>
#include <limits>
#include "Error.hpp"

Code::Parser::Parser(std::string const &code) : m_current(code.begin()), m_end(code.end()), m_column(0), m_row(0)
{
}

std::vector<std::unique_ptr<Code::Token>> Code::Parser::parseTokens()
{
    std::vector<std::unique_ptr<Token>> tokens;
    while (!isAtTheEnd())
    {
        skipComments();
        if (std::unique_ptr<KeywordToken> keyword = parseKeyword(); keyword != nullptr)
        {
            tokens.push_back(std::move(keyword));
        }
        else if (std::unique_ptr<InstructionToken> instruction = parseInstruction(); instruction != nullptr)
        {
            tokens.push_back(std::move(instruction));
        }
        else if (std::unique_ptr<AssetRefToken> assetRef = parseAssetRef(); assetRef != nullptr)
        {
            tokens.push_back(std::move(assetRef));
        }
        else if (std::unique_ptr<StringToken> str = parseString(); str != nullptr)
        {
            tokens.push_back(std::move(str));
        }
        else if (std::unique_ptr<LabelToken> label = parseLabel(); label != nullptr)
        {
            tokens.push_back(std::move(label));
        }
        else if (std::unique_ptr<IdToken> id = parseId(); id != nullptr)
        {
            tokens.push_back(std::move(id));
        }

        else if (std::unique_ptr<FloatToken> floatVal = parseFloat(); floatVal != nullptr)
        {
            tokens.push_back(std::move(floatVal));
        }
        else if (std::unique_ptr<IntToken> intVal = parseInt(); intVal != nullptr)
        {
            tokens.push_back(std::move(intVal));
        }
        else if (std::unique_ptr<SeparatorToken> sep = parseSeparator(); sep != nullptr)
        {
            tokens.push_back(std::move(sep));
        }
        else
        {
            throw Errors::ParsingError(m_row, m_column, "Unexpected symbol");
        }
        skipComments();
    }
    return tokens;
}

void Code::Parser::advance()
{

    if (m_current != m_end)
    {
        if (*m_current == '\n')
        {
            m_column = 0;
            m_row++;
        }
        else
        {
            m_column++;
        }
        m_current++;
    }
}

void Code::Parser::advanceBy(size_t offset)
{
    for (size_t i = 0; i < offset; i++)
    {
        advance();
    }
}

void Code::Parser::skipWhitespace()
{
    while (!isAtTheEnd() && (std::isspace(*m_current) && *m_current != '\n'))
    {
        advance();
    }
}

void Code::Parser::skipComments()
{
    skipWhitespace();
    if (std::optional<char> ch = getCurrent(); ch.has_value() && ch.value() == '#')
    {
        for (; getCurrent().has_value() && getCurrent().value() != '\n'; advance())
        {
        }
        advance();
    }
}

std::optional<char> Code::Parser::getFromCurrentWithOffset(size_t offset) const
{
    if (m_current + offset == m_end)
    {
        return {};
    }
    return *(m_current + offset);
}

bool Code::Parser::checkString(std::string const &str)
{
    for (size_t i = 0; i < str.size(); i++)
    {
        if (std::optional<char> ch = getFromCurrentWithOffset(i); !ch.has_value() || ch.value() != str[i])
        {
            return false;
        }
    }
    return true;
}

std::optional<Code::SpecialCharacter> Code::Parser::tryParseSpecialCharacter()
{

    // if we can't even advance far enough we don''t bother checking
    if (m_current + 1 == m_end)
    {
        return {};
    }
    std::vector<SpecialCharacter>::const_iterator charIt = std::find_if(
        SpecialCharacters.begin(),
        SpecialCharacters.end(),
        [this](SpecialCharacter const &ch)
        {
            for (size_t i = 0; i < 2; i++)
            {
                if (std::optional<char> val = getFromCurrentWithOffset(i); !val.has_value() || val.value() != ch.sequence[i])
                {
                    return false;
                }
            }
            return true;
        });
    return charIt == SpecialCharacters.end() ? std::optional<SpecialCharacter>{} : *charIt;
}

std::optional<char> Code::Parser::getCurrent()
{
    return isAtTheEnd() ? std::optional<char>{} : *m_current;
}

std::unique_ptr<Code::KeywordToken> Code::Parser::parseKeyword()
{
    for (std::unordered_map<std::string, Keyword>::const_iterator it = Keywords.begin(); it != Keywords.end(); it++)
    {
        if (checkString(it->first))
        {
            if (std::optional<char> ch = getFromCurrentWithOffset(it->first.size()); ch.has_value() && std::isspace(ch.value()))
            {
                advanceBy(it->first.size());
                return std::make_unique<KeywordToken>(m_row, m_column, it->second);
            }
        }
    }
    return nullptr;
}

std::unique_ptr<Code::IdToken> Code::Parser::parseId()
{
    // must start with a character or underscore
    if (isAtTheEnd() || (!std::isalpha(*m_current) && (*m_current) != '_'))
    {
        return nullptr;
    }
    std::string id;
    for (; !isAtTheEnd() && (std::isalnum(*m_current) || *m_current == '_'); advance())
    {
        id += *m_current;
    }
    if (id == "")
    {
        return nullptr;
    }
    return std::make_unique<IdToken>(m_row, m_column, id);
}

std::unique_ptr<Code::LabelToken> Code::Parser::parseLabel()
{
    // must start with a character or underscore
    if (std::optional<char> ch = getCurrent(); !ch.has_value() || ch.value() != '%')
    {
        return nullptr;
    }
    advance();
    std::string id;
    for (; !isAtTheEnd() && (std::isalnum(*m_current) || *m_current == '_') && *m_current != ':'; advance())
    {
        id += *m_current;
    }
    if (id == "")
    {
        throw Errors::ParsingError(m_column, m_row, "Expected label name");
    }
    return std::make_unique<LabelToken>(m_row, m_column, id);
}

std::unique_ptr<Code::StringToken> Code::Parser::parseString()
{
    if (isAtTheEnd() || *m_current != '"')
    {
        return nullptr;
    }
    advance();
    std::string result;
    for (; !isAtTheEnd() && (*m_current != '"'); advance())
    {
        if (std::optional<SpecialCharacter> spec = tryParseSpecialCharacter(); spec.has_value())
        {
            result += spec.value().character;
            advance();
        }
        else
        {
            result += *m_current;
        }
    }
    if (isAtTheEnd() || *m_current != '"')
    {
        throw Errors::ParsingError(m_row, m_column, "Expected '\"'");
    }
    advance();
    return std::make_unique<StringToken>(m_row, m_column, result);
}

std::unique_ptr<Code::AssetRefToken> Code::Parser::parseAssetRef()
{
    if (std::optional<char> ch = getCurrent(); !ch.has_value() || ch.value() != '@')
    {
        return nullptr;
    }
    advance();
    std::string id;
    for (; !isAtTheEnd() && (std::isalnum(*m_current) || *m_current == '_'); advance())
    {
        id += *m_current;
    }
    if (id == "")
    {
        throw Errors::ParsingError(m_column, m_row, "Expected asset name");
    }
    return std::make_unique<AssetRefToken>(m_row, m_column, id);
}

std::unique_ptr<Code::SeparatorToken> Code::Parser::parseSeparator()
{
    std::optional<char> curr = getCurrent();
    if (!curr.has_value())
    {
        return nullptr;
    }
    for (std::pair<char, Separator> const &sep : Separators)
    {
        if (sep.first == curr.value())
        {
            advance();
            return std::make_unique<SeparatorToken>(m_row, m_column, sep.second);
        }
    }
    return nullptr;
}

std::unique_ptr<Code::IntToken> Code::Parser::parseInt()
{
    if (std::optional<char> curr = getCurrent(); !curr.has_value() || !(std::isdigit(curr.value())) && (curr.value() != '-'))
    {
        return nullptr;
    }
    std::string num;
    size_t offset = 0;
    bool hasSeparator = false;
    for (; getFromCurrentWithOffset(offset).has_value() && (std::isdigit(getFromCurrentWithOffset(offset).value())); offset++)
    {
        num.push_back(getFromCurrentWithOffset(offset).value());
    }
    int64_t numVal;
    try
    {
        numVal = std::stol(num);
    }
    catch (std::invalid_argument const &e)
    {
        return nullptr;
    }
    catch (std::out_of_range const &e)
    {
        throw Errors::ParsingError(
            m_row, m_column,
            "Constant number is too large, valid range is " +
                std::to_string(std::numeric_limits<int64_t>::min()) +
                "< x < " +
                std::to_string(std::numeric_limits<int64_t>::max()));
    }
    advanceBy(num.size());
    return std::make_unique<IntToken>(m_row, m_column, numVal);
}

std::unique_ptr<Code::FloatToken> Code::Parser::parseFloat()
{
    if (std::optional<char> curr = getCurrent(); !curr.has_value() || !(std::isdigit(curr.value())) && (curr.value() != '-'))
    {
        return nullptr;
    }
    std::string num;
    size_t offset = 0;
    bool hasSeparator = false;
    for (; getFromCurrentWithOffset(offset).has_value() &&
           (std::isdigit(getFromCurrentWithOffset(offset).value()) || (getFromCurrentWithOffset(offset).value()) == '.');
         offset++)
    {
        if ((getFromCurrentWithOffset(offset).value()) == '.')
        {
            if (!hasSeparator)
            {
                hasSeparator = true;
            }
            else
            {
                throw Errors::ParsingError(m_row, m_column, "Malformed floating point constant");
            }
        }
        num.push_back(getFromCurrentWithOffset(offset).value());
    }
    if (!hasSeparator)
    {
        return nullptr;
    }
    float numVal;
    try
    {
        numVal = std::stod(num);
    }
    catch (std::invalid_argument const &e)
    {
        return nullptr;
    }
    catch (std::out_of_range const &e)
    {
        throw Errors::ParsingError(
            m_row, m_column,
            "Constant number is too large, valid range is " +
                std::to_string(std::numeric_limits<double>::min()) +
                "< x < " +
                std::to_string(std::numeric_limits<double>::max()));
    }
    advanceBy(num.size());
    return std::make_unique<FloatToken>(m_row, m_column, numVal);
}

std::unique_ptr<Code::InstructionToken> Code::Parser::parseInstruction()
{
    for (std::unordered_map<std::string, FusionInstruction>::const_iterator it = FusionInstructions.begin(); it != FusionInstructions.end(); it++)
    {
        if (checkString(it->first))
        {
            if (std::optional<char> ch = getFromCurrentWithOffset(it->first.size()); ch.has_value() && std::isspace(ch.value()))
            {
                advanceBy(it->first.size());
                return std::make_unique<InstructionToken>(m_row, m_column, it->second);
            }
        }
    }
    return nullptr;
    return std::unique_ptr<InstructionToken>();
}
