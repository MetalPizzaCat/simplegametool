#include "CodeGenerator.hpp"
#include "Error.hpp"

void Code::Fusion::FusionCodeGenerator::generate()
{
    while (isSeparator(Separator::EndOfStatement))
    {
        advance();
    }
    while (isKeyword(Keyword::Type) || isKeyword(Keyword::Function))
    {

        if (isKeyword(Keyword::Type))
        {
            parseTypeDeclaration();
        }
        else if (isKeyword(Keyword::Function))
        {
            parseFunctionDeclaration();
        }
    }
}

Code::Token const *Code::Fusion::FusionCodeGenerator::getCurrent() const
{
    return m_it == m_tokens.end() ? nullptr : m_it->get();
}

bool Code::Fusion::FusionCodeGenerator::isKeyword(Keyword keyword) const
{
    if (KeywordToken const *key = dynamic_cast<KeywordToken const *>(getCurrent()); key != nullptr && key->getKeyword() == keyword)
    {
        return true;
    }
    return false;
}

bool Code::Fusion::FusionCodeGenerator::isSeparator(Separator separator) const
{
    if (SeparatorToken const *sep = dynamic_cast<SeparatorToken const *>(getCurrent()); sep != nullptr && sep->getSeparator() == separator)
    {
        return true;
    }
    return false;
}

void Code::Fusion::FusionCodeGenerator::parseTypeDeclaration()
{
    using namespace Engine::Runnable;
    consumeKeyword(Keyword::Type, "Expected 'type'");
    IdToken const *name = getTokenOrError<IdToken>("expected type name");
    advance();
    consumeSeparator(Separator::BlockOpen, "expected '{'");
    optionallyConsumeSeparator(Separator::EndOfStatement);
    consumeIdSeparator("sprite", "Expected sprite info");
    consumeSeparator(Separator::Equals, "Expected '='");
    AssetRefToken const *spriteName = getTokenOrError<AssetRefToken>("Expected sprite name");
    advance();
    std::map<std::string, CodeConstantValue> fields;
    IdToken const *fieldTok = nullptr;
    while ((fieldTok = dynamic_cast<IdToken const *>(getCurrent())) != nullptr)
    {
        advance();
        consumeSeparator(Separator::Equals, "Expected '='");
        CodeConstantValue val = parseConstant("Expected value");
        advance();
        fields[fieldTok->getId()] = val;

        if (isSeparator(Separator::EndOfStatement))
        {
            advance();
        }
    }

    consumeSeparator(Separator::BlockClose, "expected '}'");

    m_builder.addType(TypeInfo(name->getId(), spriteName->getAssetName(), fields));
}

void Code::Fusion::FusionCodeGenerator::parseFunctionDeclaration()
{
}

void Code::Fusion::FusionCodeGenerator::consumeKeyword(Keyword keyword, std::string const &errorMessage)
{
    if (getTokenOrError<KeywordToken>(errorMessage)->getKeyword() != keyword)
    {
        error(errorMessage);
    }
    advance();
}

void Code::Fusion::FusionCodeGenerator::consumeSeparator(Separator separator, std::string const &errorMessage)
{
    if (getTokenOrError<SeparatorToken>(errorMessage)->getSeparator() != separator)
    {
        error(errorMessage);
    }
    advance();
}

void Code::Fusion::FusionCodeGenerator::consumeIdSeparator(std::string const &id, std::string const &errorMessage)
{
    if (getTokenOrError<IdToken>(errorMessage)->getId() != id)
    {
        error(errorMessage);
    }
    advance();
}

Engine::Runnable::CodeConstantValue Code::Fusion::FusionCodeGenerator::parseConstant(std::string const &errorMessage)
{
    if (isOfType<IntToken>())
    {
        return getTokenOrError<IntToken>("")->getValue();
    }
    if (isOfType<FloatToken>())
    {
        return getTokenOrError<FloatToken>("")->getValue();
    }
    if (isOfType<StringToken>())
    {
        return m_builder.getOrAddStringId(getTokenOrError<StringToken>("")->getString());
    }
    if (isSeparator(Separator::BracketOpen))
    {
        advance();
        double a = 0;
        if (isOfType<FloatToken>())
        {
            a = getTokenOrError<FloatToken>("Expected number")->getValue();
        }
        else if (isOfType<IntToken>())
        {
            a = (double)getTokenOrError<IntToken>("Expected number")->getValue();
        }
        else
        {
            error("Expected number");
        }
        advance();
        consumeSeparator(Separator::Comma, "Expected ','");
        double b = 0;
        if (isOfType<FloatToken>())
        {
            b = getTokenOrError<FloatToken>("Expected number")->getValue();
        }
        else if (isOfType<IntToken>())
        {
            b = (double)getTokenOrError<IntToken>("Expected number")->getValue();
        }
        else
        {
            error("Expected number");
        }
        advance();
         consumeSeparator(Separator::BracketClose, "Expected ')'");
        return sf::Vector2f(a, b);
    }
    error("Unexpected symbol, expected a constant values");
    return 0;
}

void Code::Fusion::FusionCodeGenerator::optionallyConsumeSeparator(Separator separator)
{
    if (isSeparator(separator))
    {
        advance();
    }
}

void Code::Fusion::FusionCodeGenerator::error(std::string const &errorMessage)
{
    Token const *t = getCurrent();
    if (t == nullptr)
    {
        t = (m_it - 1)->get();
    }
    throw Errors::ParsingError(t->getRow(), t->getColumn(), errorMessage);
}
