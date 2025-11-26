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
        return "CreateInstance";
    case FusionInstruction::Set:
        return "Set";
    case FusionInstruction::Get:
        return "Get";
    case FusionInstruction::Add:
        return "Add";
    case FusionInstruction::Sub:
        return "Sub";
    case FusionInstruction::Mul:
        return "Mul";
    case FusionInstruction::Div:
        return "Div";
    case FusionInstruction::Call:
        return "Call";
    case FusionInstruction::GetPosition:
        return "GetPosition";
    case FusionInstruction::SetPosition:
        return "SetPosition";
    case FusionInstruction::GetPositionX:
        return "GetPositionX";
    case FusionInstruction::SetPositionX:
        return "SetPositionX";
    case FusionInstruction::MakeVector:
        return "MakeVector";
    }
    return "Unknown";
}

std::string Code::StringToken::toString() const
{
    return std::string("STR(") + m_str + ")";
}

std::string Code::IntToken::toString() const
{
    return std::to_string(m_number);
}

std::string Code::FloatToken::toString() const
{
    return std::to_string(m_number);
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
    default:
        return "UNKNOWN";
    }
}

std::string Code::AssetRefToken::toString() const
{
    return std::string("ASSET(") + m_assetName + ")";
}
