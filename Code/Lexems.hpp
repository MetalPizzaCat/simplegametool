#pragma once
#include <string>
#include <unordered_map>
#include <map>
#include <vector>

namespace Code
{
    enum class FusionInstruction
    {
        CreateInstance,
        Set,
        Get,
        Add,
        Sub,
        Mul,
        Div,
        Call,
        GetPosition,
        SetPosition,
        GetPositionX,
        SetPositionX,
        MakeVector,
    };

    enum class Keyword
    {
        Function,
        End,
        Type,
        Of,
    };

    enum class Separator
    {
        Colon,
        EndOfStatement,
        BlockOpen,
        BlockClose,
        Equals,
        BracketOpen,
        BracketClose,
        Comma,
    };

    static const std::unordered_map<std::string, Keyword> Keywords = {
        {"func", Keyword::Function},
        {"end", Keyword::End},
        {"type", Keyword::Type},
        {"of", Keyword::Of}};

    static const std::unordered_map<std::string, FusionInstruction> FusionInstructions = {
        {"create_instance", FusionInstruction::CreateInstance},
        {"set", FusionInstruction::Set},
        {"get", FusionInstruction::Get},
        {"add", FusionInstruction::Add},
        {"sub", FusionInstruction::Sub},
        {"mul", FusionInstruction::Mul},
        {"div", FusionInstruction::Div},
        {"call", FusionInstruction::Call},
        {"get_position", FusionInstruction::GetPosition},
        {"set_position", FusionInstruction::SetPosition},
        {"get_x", FusionInstruction::GetPositionX},
        {"get_y", FusionInstruction::SetPositionX},
        {"make_vector", FusionInstruction::MakeVector},
    };

    static const std::map<char, Separator> Separators = {
        {':', Separator::Colon},
        {',', Separator::Comma},
        {'{', Separator::BlockOpen},
        {'}', Separator::BlockClose},
        {'=', Separator::Equals},
        {'(', Separator::BracketOpen},
        {')', Separator::BracketClose},
        {'\n', Separator::EndOfStatement},
        {';', Separator::EndOfStatement}};

    struct SpecialCharacter
    {
        const char *sequence;
        char character;
    };

    static const std::vector<SpecialCharacter> SpecialCharacters = {
        SpecialCharacter{.sequence = "\\n", .character = '\n'},
        SpecialCharacter{.sequence = "\\\"", .character = '\"'},
        SpecialCharacter{.sequence = "\\'", .character = '\''},
        SpecialCharacter{.sequence = "\\t", .character = '\t'},
        SpecialCharacter{.sequence = "\\\\", .character = '\\'},
    };
}