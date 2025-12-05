#include "Token.hpp"

std::string Code::KeywordToken::toString() const
{
    switch (m_keyword)
    {
    case Keyword::Function:
        return "Keyword(Function)";
    case Keyword::End:
        return "Keyword(End)";
    case Keyword::Type:
        return "Keyword(Type)";
    case Keyword::Of:
        return "Keyword(Of)";
    default:
        return "UNKNOWN";
    }
}

std::string Code::InstructionToken::toString() const
{
    switch (m_instruction)
    {
    case FusionInstruction::CreateInstance:
        return "OP(CreateInstance)";
    case FusionInstruction::Set:
        return "OP(Set)";
    case FusionInstruction::Get:
        return "OP(Get)";
    case FusionInstruction::Add:
        return "OP(Add)";
    case FusionInstruction::Sub:
        return "OP(Sub)";
    case FusionInstruction::Mul:
        return "OP(Mul)";
    case FusionInstruction::Div:
        return "OP(Div)";
    case FusionInstruction::Call:
        return "OP(Call)";
    case FusionInstruction::GetPosition:
        return "OP(GetPosition)";
    case FusionInstruction::SetPosition:
        return "OP(SetPosition)";
    case FusionInstruction::GetPositionX:
        return "OP(GetPositionX)";
    case FusionInstruction::SetPositionX:
        return "OP(SetPositionX)";
    case FusionInstruction::MakeVector:
        return "OP(MakeVector)";
    case FusionInstruction::Push:
        return "OP(Push)";
    case FusionInstruction::Jump:
        return "OP(Jump)";
    case FusionInstruction::JumpIf:
        return "OP(JumpIf)";
    case FusionInstruction::Print:
        return "OP(Print)";
    case FusionInstruction::Equals:
        return "OP(Equals)";
    case FusionInstruction::NotEquals:
        return "OP(NotEquals)";
    case FusionInstruction::More:
        return "OP(More)";
    case FusionInstruction::Less:
        return "OP(Less)";
    case FusionInstruction::MoreOrEquals:
        return "OP(MoreOrEquals)";
    case FusionInstruction::LessOrEquals:
        return "OP(LessOrEquals)";
    }
    return "Unknown";
}

std::string Code::StringToken::toString() const
{
    return std::string("STR(") + m_str + ")";
}

std::string Code::IntToken::toString() const
{
    return std::string("INT(") + std::to_string(m_number) + ")";
}

std::string Code::FloatToken::toString() const
{
    return std::string("FLOAT(") + std::to_string(m_number) + ")";
}

std::string Code::BoolToken::toString() const
{
    return (m_val ? "true" : "false");
}

std::string Code::IdToken::toString() const
{
    return std::string("ID(") + m_str + ")";
}

std::string Code::SeparatorToken::toString() const
{
    switch (m_separator)
    {
    case Separator::Colon:
        return "SEP(Colon)";
    case Separator::BlockOpen:
        return "SEP(BlockOpen)";
    case Separator::BlockClose:
        return "SEP(BlockClose)";
    case Separator::Equals:
        return "SEP(Equals)";
    case Separator::BracketOpen:
        return "SEP(BracketOpen)";
    case Separator::BracketClose:
        return "SEP(BracketClose)";
    case Separator::EndOfStatement:
        return "SEP(EndOfStatement)";
    case Separator::Comma:
        return "SEP(Comma)";
    default:
        return "UNKNOWN";
    }
}

std::string Code::AssetRefToken::toString() const
{
    return std::string("ASSET(") + m_assetName + ")";
}

std::string Code::LabelToken::toString() const
{
    return std::string("LABEL(") + m_str + ")";
}

std::string Code::VariableToken::toString() const
{
    return std::string("VAR(") + m_id + ")";
}
