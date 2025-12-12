#include "CodeGenerator.hpp"
#include "Error.hpp"

Code::Fusion::FusionCodeGenerator::FusionCodeGenerator(std::vector<std::unique_ptr<Token>> tokens, std::vector<std::string> const &defaultTypes)
    : m_tokens(std::move(tokens)), m_it(m_tokens.begin()), m_builder(defaultTypes)
{
}

void Code::Fusion::FusionCodeGenerator::generate()
{
    consumeEndOfStatement();
    while (isKeyword(Keyword::Type) || isKeyword(Keyword::Function))
    {
        if (isKeyword(Keyword::Type))
        {
            parseTypeDeclaration();
        }
        else if (isKeyword(Keyword::Function))
        {
            auto f = parseFunctionDeclaration();
            m_builder.addFunction(f.first, f.second);
        }
        consumeEndOfStatement();
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

    if (!m_builder.addTypeDeclarationLocation(name->getId(), Debug::DebugInfoSourceData{.row = name->getRow(), .column = name->getColumn()}))
    {
        error("Type with name '" + name->getId() + "' already exists");
    }
    advance();
    consumeSeparator(Separator::BlockOpen, "expected '{'");
    optionallyConsumeSeparator(Separator::EndOfStatement);
    consumeIdSeparator("sprite", "Expected sprite info");
    consumeSeparator(Separator::Equals, "Expected '='");
    AssetRefToken const *spriteName = getTokenOrError<AssetRefToken>("Expected sprite name");
    advance();
    consumeEndOfStatement();
    std::unordered_map<std::string, CodeConstantValue> fields;
    std::unordered_map<std::string, CodeConstantValue> constants;

    while (isOfType<IdToken>() || isKeyword(Keyword::Const))
    {
        bool isConst = false;
        if (isKeyword(Keyword::Const))
        {
            isConst = true;
        }
        IdToken const *fieldTok = getTokenOrError<IdToken>("Expected field name");
        advance();
        consumeSeparator(Separator::Equals, "Expected '='");
        CodeConstantValue val = parseConstant("Expected value");
        advance();
        if (isConst)
        {
            constants[fieldTok->getId()] = val;
        }
        else
        {
            fields[fieldTok->getId()] = val;
        }
        consumeEndOfStatement();
    }
    std::unordered_map<std::string, RunnableFunction> methods;
    // parse methods
    while (isKeyword(Keyword::Function))
    {
        auto method = parseFunctionDeclaration(m_builder.getNextTypeId());
        methods[method.first] = method.second;
        consumeEndOfStatement();
    }

    consumeSeparator(Separator::BlockClose, "expected '}'");

    // times likes this is when i miss how rust handles errors, but alas i can't be bothered to fight borrow checker
    // we check fot this twice because default types don't have debug file location, because they are not in any file
    if (m_builder.addType(TypeInfo(name->getId(), spriteName->getAssetName(), fields, constants, methods, false)) == (size_t)-1)
    {
        throw Errors::ParsingError(name->getRow(), name->getColumn(), "Type with name '" + name->getId() + "' already exists");
    }
}

std::pair<std::string, Engine::Runnable::RunnableFunction> Code::Fusion::FusionCodeGenerator::parseFunctionDeclaration(size_t typeId)
{
    using namespace Engine::Runnable;
    consumeKeyword(Keyword::Function, "Expected 'func'");
    IdToken const *name = getTokenOrError<IdToken>("expected function name");
    Debug::FunctionDebugInfo &debugInfo = m_builder.getOrCreateDebugEntryForFunction(typeId, name->getId());
    advance();
    m_builder.createBlock();
    std::vector<std::string> argumentNames;
    if (isSeparator(Separator::BracketOpen))
    {
        advance();
        while (getCurrent() != nullptr)
        {
            argumentNames.push_back(getTokenOrError<IdToken>("Expected argument name")->getId());
            m_builder.getCurrentBlock().addVariableName(getTokenOrError<IdToken>("Expected argument name")->getId());
            advance();
            if (isSeparator(Separator::Comma))
            {
                advance();
            }
            else if (isSeparator(Separator::BracketClose))
            {
                break;
            }
        }
        consumeSeparator(Separator::BracketClose, "Expected ')'");
    }
    consumeSeparator(Separator::BlockOpen, "expected '{'");
    consumeEndOfStatement();

    while (getCurrent() != nullptr)
    {
        if (isOfType<SeparatorToken>())
        {
            break;
        }
        if (isOfType<LabelToken>())
        {
            LabelToken const *t = getTokenOrError<LabelToken>("Expected label");
            advance();
            consumeSeparator(Separator::Colon, "Expected ':' at the end of label declaration");
            if (m_builder.getCurrentBlock().labelExists(t->getId()))
            {
                error("Jump label redefinition");
            }
            m_builder.getCurrentBlock().addLabelPosition(t->getId(), m_builder.getCurrentBlock().getBytes().size());
            consumeEndOfStatementOrError("Expected new line or ';'");
            continue;
        }
        InstructionToken const *token = getTokenOrError<InstructionToken>("Expected instruction");
        advance();
        // push is only special instruction type because each variant is its own opcode
        // while we could make the data structure more complex and make it find specific combination of instruction and arguments
        // this wouldn't be worth it, although could be worth to take a look in future if necessary
        if (token->getInstruction() == FusionInstruction::Push)
        {
            std::vector<uint8_t> bytes;
            Engine::Runnable::CodeConstantValue val = parseConstant("Expected valid value");
            switch ((Engine::Runnable::CodeConstantValueType)val.index())
            {
            case Engine::Runnable::CodeConstantValueType::Bool:
                if (std::get<bool>(val))
                {
                    bytes.push_back((uint8_t)Engine::Instructions::PushTrue);
                }
                else
                {
                    bytes.push_back((uint8_t)Engine::Instructions::PushFalse);
                }
                break;
            case Engine::Runnable::CodeConstantValueType::Int:
            {
                bytes = {(uint8_t)Engine::Instructions::PushInt};
                std::vector<uint8_t> temp = parseToBytes(std::get<int64_t>(val));
                bytes.insert(bytes.end(), temp.begin(), temp.end());
            }
            break;
            case Engine::Runnable::CodeConstantValueType::Float:
            {
                bytes = {(uint8_t)Engine::Instructions::PushFloat};
                std::vector<uint8_t> temp = parseToBytes(std::get<double>(val));
                bytes.insert(bytes.end(), temp.begin(), temp.end());
            }
            break;
            case Engine::Runnable::CodeConstantValueType::StringId:
            {
                bytes = {(uint8_t)Engine::Instructions::LoadConstString};
                std::vector<uint8_t> temp = parseToBytes<size_t>(std::get<size_t>(val));
                bytes.insert(bytes.end(), temp.begin(), temp.end());
            }
            break;
            case Engine::Runnable::CodeConstantValueType::Vector:
            {
                bytes = {(uint8_t)Engine::Instructions::PushVector};
                sf::Vector2f vec = std::get<sf::Vector2f>(val);
                std::vector<uint8_t> temp = parseToBytes((double)vec.x);
                bytes.insert(bytes.end(), temp.begin(), temp.end());
                temp = parseToBytes((double)vec.y);
                bytes.insert(bytes.end(), temp.begin(), temp.end());
                backtrack();
            }
            break;
            }
            advance();
            m_builder.getCurrentBlock().insert(bytes);
            debugInfo.addByteRangeFromPrevious(bytes.size(), token->getRow(), token->getColumn());
            consumeEndOfStatementOrError("Expected new line or ';'");
        }
        else if (token->getInstruction() == FusionInstruction::Vars)
        {
            while (isOfType<IdToken>())
            {
                m_builder.getCurrentBlock().addVariableName(getTokenOrError<IdToken>("Expected variable name")->getId());
                advance();
                if (isSeparator(Separator::Comma))
                {
                    advance();
                }
            }
            consumeEndOfStatementOrError("expected new line or ';'");
        }
        // jump is handled entirely differently because it works using relative offsets
        else if (token->getInstruction() == FusionInstruction::Jump || token->getInstruction() == FusionInstruction::JumpIf)
        {
            LabelToken const *dest = getTokenOrError<LabelToken>("Expected destination for jump");
            advance();
            consumeEndOfStatementOrError("Expected new line or ';'");
            if (token->getInstruction() == FusionInstruction::Jump)
            {
                m_builder.getCurrentBlock().insert({(uint8_t)Engine::Instructions::JumpBy});
            }
            else
            {
                m_builder.getCurrentBlock().insert({(uint8_t)Engine::Instructions::JumpByIf});
            }
            m_builder.getCurrentBlock().addLabelDestination(dest->getId(), m_builder.getCurrentBlock().getBytes().size(), dest->getColumn(), dest->getRow());

            m_builder.getCurrentBlock().reserveBytesForJump();
            debugInfo.addByteRangeFromPrevious(1 + sizeof(Engine::JumpDistanceType), token->getRow(), token->getColumn());
        }
        else
        {
            parseInstruction(token->getInstruction(), debugInfo);
        }
    }

    consumeSeparator(Separator::BlockClose, "expected '}'");
    m_builder.getCurrentBlock().applyLabels();
    std::vector<uint8_t> temp = m_builder.getCurrentBlock().getBytes();
    m_builder.popBlock();
    return std::make_pair(name->getId(), Engine::Runnable::RunnableFunction{.argumentCount = argumentNames.size(), .bytes = std::move(temp)});
}

void Code::Fusion::FusionCodeGenerator::parseInstruction(FusionInstruction instruction, Debug::FunctionDebugInfo &debugInfo)
{
    if (!FusionInstructionsData.contains(instruction))
    {
        error("Unsupported instruction '" + getFusionInstructionText(instruction) + "'");
        return;
    }
    FusionInstructionData const *data = &FusionInstructionsData.at(instruction);
    std::vector<uint8_t> bytes = {(uint8_t)data->instruction};

    Token const *token = getCurrent();
    size_t row = token->getRow();
    size_t column = token->getColumn();
    for (size_t argCount = 0; !isSeparator(Separator::EndOfStatement) && (token = getCurrent()) != nullptr; argCount++)
    {
        if (argCount >= data->argumentTypes.size())
        {
            error("Instruction received too many arguments");
        }
        switch (data->argumentTypes[argCount])
        {
        case InstructionArgumentType::Any:
        {
            std::vector<uint8_t> t = createInstructionConstValueBytes(token);
            bytes.insert(bytes.end(), t.begin(), t.end());
        }
        break;
        case InstructionArgumentType::Bool:
            bytes.push_back((uint8_t)getTokenOrError<BoolToken>("Expected boolean value")->getValue());
            break;
        case InstructionArgumentType::Int:
        {
            std::vector<uint8_t> b = parseToBytes(getTokenOrError<IntToken>("Expected integer value")->getValue());
            bytes.insert(bytes.end(), b.begin(), b.end());
        }
        break;
        case InstructionArgumentType::Float:
        {
            std::vector<uint8_t> b = parseToBytes(getTokenOrError<FloatToken>("Expected floating point value")->getValue());
            bytes.insert(bytes.end(), b.begin(), b.end());
        }
        case InstructionArgumentType::String:
        {
            size_t id = m_builder.getOrAddStringId(getTokenOrError<StringToken>("Expected string value")->getString());
            std::vector<uint8_t> b = parseToBytes(id);
            bytes.insert(bytes.end(), b.begin(), b.end());
        }
        break;
        case InstructionArgumentType::ObjectType:
        {
            if (std::optional<size_t> id = m_builder.getTypeByName(getTokenOrError<IdToken>("Expected type name")->getId()); id.has_value())
            {
                std::vector<uint8_t> b = parseToBytes(id.value());
                bytes.insert(bytes.end(), b.begin(), b.end());
            }
            else
            {
                error("Unknown type '" + getTokenOrError<IdToken>("Expected type name")->getId() + "'");
            }
        }
        break;
        case InstructionArgumentType::FunctionName:
        {
            size_t id = m_builder.getOrAddStringId(getTokenOrError<IdToken>("Expected function name")->getId());
            std::vector<uint8_t> b = parseToBytes(id);
            bytes.insert(bytes.end(), b.begin(), b.end());
        }
        break;
        case InstructionArgumentType::MethodName:
        {
            if (std::optional<size_t> id = m_builder.getTypeByName(getTokenOrError<IdToken>("Expected type name")->getId()); id.has_value())
            {
                advance();
                std::vector<uint8_t> b = parseToBytes(id.value());
                bytes.insert(bytes.end(), b.begin(), b.end());
                consumeSeparator(Separator::Colon, "Expected '::'");
                consumeSeparator(Separator::Colon, "Expected '::'");

                size_t nameId = m_builder.getOrAddStringId(getTokenOrError<IdToken>("Expected function name")->getId());
                b = parseToBytes(nameId);
                bytes.insert(bytes.end(), b.begin(), b.end());
            }
            else
            {
                error("Unknown type '" + getTokenOrError<IdToken>("Expected type name")->getId() + "'");
            }
        }
        break;
        case InstructionArgumentType::VariableName:
        {
            if (isOfType<VariableToken>())
            {
                if (std::optional<size_t> id = m_builder.getCurrentBlock().getVariableId(getTokenOrError<VariableToken>("Expected variable name")->getId()); id.has_value())
                {
                    std::vector<uint8_t> b = parseToBytes(id.value());
                    bytes.insert(bytes.end(), b.begin(), b.end());
                }
                else
                {
                    error("No variable with name '" + getTokenOrError<VariableToken>("Expected variable name")->getId() + "' is present in current block");
                }
            }
            else if (int64_t varId = getTokenOrError<IntToken>("Expected variable id")->getValue(); varId >= 0)
            {
                std::vector<uint8_t> b = parseToBytes(varId);
                bytes.insert(bytes.end(), b.begin(), b.end());
            }
            else
            {
                error("Variable id must be higher than 0");
            }
        }
        break;
        }
        advance();
        optionallyConsumeSeparator(Separator::Comma);
    }
    consumeEndOfStatementOrError("expected new line or ';'");
    debugInfo.addByteRangeFromPrevious(bytes.size(), row, column);
    m_builder.getCurrentBlock().insert(bytes);
}

std::vector<uint8_t> Code::Fusion::FusionCodeGenerator::createInstructionConstValueBytes(Token const *token)
{
    if (isOfType<IntToken>())
    {
        return parseToBytes(getTokenOrError<IntToken>("Expected integer value")->getValue());
    }
    else if (isOfType<FloatToken>())
    {
        return parseToBytes(getTokenOrError<FloatToken>("Expected floating point value")->getValue());
    }
    else if (isOfType<StringToken>())
    {
        return parseToBytes(m_builder.getOrAddStringId(getTokenOrError<StringToken>("Expected string value")->getString()));
    }
    // for vectors
    else if (isSeparator(Separator::BracketOpen))
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
        std::vector<uint8_t> bytes = parseToBytes(a);
        std::vector<uint8_t> temp = parseToBytes(b);
        bytes.insert(bytes.end(), temp.begin(), temp.end());
        return bytes;
    }

    error("Unknown const type");
    return std::vector<uint8_t>();
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
    if (isOfType<BoolToken>())
    {
        return getTokenOrError<BoolToken>("")->getValue();
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

void Code::Fusion::FusionCodeGenerator::backtrack()
{
    if (m_it != m_tokens.begin())
    {
        m_it--;
    }
}

void Code::Fusion::FusionCodeGenerator::consumeEndOfStatement()
{
    while (isSeparator(Separator::EndOfStatement))
    {
        advance();
    }
}

void Code::Fusion::FusionCodeGenerator::consumeEndOfStatementOrError(std::string const &err)
{
    if (!isSeparator(Separator::EndOfStatement))
    {
        error(err);
    }
    while (isSeparator(Separator::EndOfStatement))
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
