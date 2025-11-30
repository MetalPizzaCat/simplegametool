#pragma once
#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include "../Engine/Instructions.hpp"

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
        Push,
        Print,
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
        {"push", FusionInstruction::Push},
        {"get_pos", FusionInstruction::GetPosition},
        {"set_pos", FusionInstruction::SetPosition},
        {"get_x", FusionInstruction::GetPositionX},
        {"get_y", FusionInstruction::SetPositionX},
        {"make_vector", FusionInstruction::MakeVector},
        {"print", FusionInstruction::Print},
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

    enum class InstructionArgumentType
    {
        Any,
        Bool,
        Int,
        Float,
        String,
        ObjectType,
        FunctionName
    };

    struct FusionInstructionData
    {
        Engine::Instructions instruction;
        std::vector<InstructionArgumentType> argumentTypes;
    };

    static const std::map<FusionInstruction, FusionInstructionData> FusionInstructionsData = {
        {FusionInstruction::CreateInstance, FusionInstructionData{.instruction = Engine::Instructions::CreateInstance, .argumentTypes = {InstructionArgumentType::ObjectType}}},
        {FusionInstruction::Set, FusionInstructionData{.instruction = Engine::Instructions::SetLocal, .argumentTypes = {InstructionArgumentType::Int}}},
        {FusionInstruction::Get, FusionInstructionData{.instruction = Engine::Instructions::GetLocal, .argumentTypes = {InstructionArgumentType::Int}}},
        {FusionInstruction::Add, FusionInstructionData{.instruction = Engine::Instructions::Add, .argumentTypes = {}}},
        {FusionInstruction::Sub, FusionInstructionData{.instruction = Engine::Instructions::Sub, .argumentTypes = {}}},
        {FusionInstruction::Mul, FusionInstructionData{.instruction = Engine::Instructions::Mul, .argumentTypes = {}}},
        {FusionInstruction::Div, FusionInstructionData{.instruction = Engine::Instructions::Div, .argumentTypes = {}}},
        {FusionInstruction::Call, FusionInstructionData{.instruction = Engine::Instructions::Call, .argumentTypes = {}}},
        {FusionInstruction::GetPosition, FusionInstructionData{.instruction = Engine::Instructions::GetPosition, .argumentTypes = {}}},
        {FusionInstruction::SetPosition, FusionInstructionData{.instruction = Engine::Instructions::SetPosition, .argumentTypes = {}}},
        {FusionInstruction::GetPositionX, FusionInstructionData{.instruction = Engine::Instructions::GetVectorX, .argumentTypes = {}}},
        {FusionInstruction::MakeVector, FusionInstructionData{.instruction = Engine::Instructions::MakeVector, .argumentTypes = {}}},
        {FusionInstruction::Print, FusionInstructionData{.instruction = Engine::Instructions::Print, .argumentTypes = {}}},
    };
}