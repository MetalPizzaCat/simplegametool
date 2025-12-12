#pragma once
#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include "../Engine/Execution/Instructions.hpp"

namespace Code
{
    enum class FusionInstruction
    {
        CreateInstance,
        GetInstance,
        Set,
        Get,
        Add,
        Sub,
        Mul,
        Div,
        GetPosition,
        SetPosition,
        GetPositionX,
        GetPositionY,
        MakeVector,
        Push,
        Jump,
        JumpIf,
        Print,
        Equals,
        NotEquals,
        Not,
        More,
        Less,
        MoreOrEquals,
        LessOrEquals,
        Call,
        CallMethod,
        CallMethodStatic,
        Return,
        End,
        GetField,
        SetField,
        HasField,
        GetConst,
        // won't actually work as instruction, as no byte code will be generated, however it is easier to include it into this system
        Vars,
        CreateSoundPlayer,
        PlaySound,
        GetSize,
        SetSize,
        AreOverlapping,
        CreateLabel,
        ToString,
        ToInt,
        ToFloat,
        SetGlobal,
        GetGlobal,
    };

    enum class Keyword
    {
        Function,
        End,
        Type,
        Of,
        Const,
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
        {"type", Keyword::Type},
        {"of", Keyword::Of},
        {"const", Keyword::Const}};

    static const std::unordered_map<std::string, FusionInstruction> FusionInstructions = {
        {"create_instance", FusionInstruction::CreateInstance},
        {"get_instance", FusionInstruction::GetInstance},
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
        {"get_y", FusionInstruction::GetPositionY},
        {"make_vector", FusionInstruction::MakeVector},
        {"jump", FusionInstruction::Jump},
        {"jump_if", FusionInstruction::JumpIf},
        {"print", FusionInstruction::Print},
        {"eq", FusionInstruction::Equals},
        {"neq", FusionInstruction::NotEquals},
        {"not", FusionInstruction::Not},
        {"less", FusionInstruction::Less},
        {"more", FusionInstruction::More},
        {"more_eq", FusionInstruction::MoreOrEquals},
        {"less_eq", FusionInstruction::LessOrEquals},
        {"call", FusionInstruction::Call},
        {"call_method", FusionInstruction::CallMethod},
        {"call_method_static", FusionInstruction::CallMethodStatic},
        {"ret", FusionInstruction::Return},
        {"end", FusionInstruction::End},
        {"get_field", FusionInstruction::GetField},
        {"set_field", FusionInstruction::SetField},
        {"vars", FusionInstruction::Vars},
        {"get_const", FusionInstruction::GetConst},
        {"create_sound_player", FusionInstruction::CreateSoundPlayer},
        {"play_sound", FusionInstruction::PlaySound},
        {"get_size", FusionInstruction::GetSize},
        {"set_size", FusionInstruction::SetSize},
        {"are_overlapping", FusionInstruction::AreOverlapping},
        {"end", FusionInstruction::End},
        {"create_label", FusionInstruction::CreateLabel},
        {"to_string", FusionInstruction::ToString},
        {"to_int", FusionInstruction::ToInt},
        {"to_float", FusionInstruction::ToFloat},
        {"set_global", FusionInstruction::SetGlobal},
        {"get_global", FusionInstruction::GetGlobal},
    };

    std::string getFusionInstructionText(FusionInstruction instruction);

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
        FunctionName,
        MethodName,
        VariableName,
    };

    struct FusionInstructionData
    {
        Engine::Instructions instruction;
        std::vector<InstructionArgumentType> argumentTypes;
    };

    static const std::map<FusionInstruction, FusionInstructionData> FusionInstructionsData = {
        {FusionInstruction::CreateInstance, FusionInstructionData{.instruction = Engine::Instructions::CreateInstance, .argumentTypes = {InstructionArgumentType::ObjectType}}},
        {FusionInstruction::GetInstance, FusionInstructionData{.instruction = Engine::Instructions::GetInstanceByName, .argumentTypes = {}}},
        {FusionInstruction::Set, FusionInstructionData{.instruction = Engine::Instructions::SetLocal, .argumentTypes = {InstructionArgumentType::VariableName}}},
        {FusionInstruction::Get, FusionInstructionData{.instruction = Engine::Instructions::GetLocal, .argumentTypes = {InstructionArgumentType::VariableName}}},
        {FusionInstruction::Add, FusionInstructionData{.instruction = Engine::Instructions::Add, .argumentTypes = {}}},
        {FusionInstruction::Sub, FusionInstructionData{.instruction = Engine::Instructions::Sub, .argumentTypes = {}}},
        {FusionInstruction::Mul, FusionInstructionData{.instruction = Engine::Instructions::Mul, .argumentTypes = {}}},
        {FusionInstruction::Div, FusionInstructionData{.instruction = Engine::Instructions::Div, .argumentTypes = {}}},
        {FusionInstruction::GetPosition, FusionInstructionData{.instruction = Engine::Instructions::GetPosition, .argumentTypes = {}}},
        {FusionInstruction::SetPosition, FusionInstructionData{.instruction = Engine::Instructions::SetPosition, .argumentTypes = {}}},
        {FusionInstruction::GetPositionX, FusionInstructionData{.instruction = Engine::Instructions::GetVectorX, .argumentTypes = {}}},
        {FusionInstruction::GetPositionY, FusionInstructionData{.instruction = Engine::Instructions::GetVectorY, .argumentTypes = {}}},
        {FusionInstruction::MakeVector, FusionInstructionData{.instruction = Engine::Instructions::MakeVector, .argumentTypes = {}}},
        {FusionInstruction::Print, FusionInstructionData{.instruction = Engine::Instructions::Print, .argumentTypes = {}}},
        {FusionInstruction::Equals, FusionInstructionData{.instruction = Engine::Instructions::Equals, .argumentTypes = {}}},
        {FusionInstruction::NotEquals, FusionInstructionData{.instruction = Engine::Instructions::NotEquals, .argumentTypes = {}}},
        {FusionInstruction::More, FusionInstructionData{.instruction = Engine::Instructions::More, .argumentTypes = {}}},
        {FusionInstruction::Less, FusionInstructionData{.instruction = Engine::Instructions::Less, .argumentTypes = {}}},
        {FusionInstruction::MoreOrEquals, FusionInstructionData{.instruction = Engine::Instructions::MoreOrEquals, .argumentTypes = {}}},
        {FusionInstruction::LessOrEquals, FusionInstructionData{.instruction = Engine::Instructions::LessOrEquals, .argumentTypes = {}}},
        {FusionInstruction::Return, FusionInstructionData{.instruction = Engine::Instructions::Return, .argumentTypes = {}}},
        {FusionInstruction::End, FusionInstructionData{.instruction = Engine::Instructions::ExitFunction, .argumentTypes = {}}},
        {FusionInstruction::Call, FusionInstructionData{.instruction = Engine::Instructions::CallFunction, .argumentTypes = {InstructionArgumentType::FunctionName}}},
        {FusionInstruction::CallMethod, FusionInstructionData{.instruction = Engine::Instructions::CallMethod, .argumentTypes = {InstructionArgumentType::FunctionName}}},
        {FusionInstruction::CallMethodStatic, FusionInstructionData{.instruction = Engine::Instructions::CallMethodStatic, .argumentTypes = {InstructionArgumentType::MethodName}}},
        {FusionInstruction::Return, FusionInstructionData{.instruction = Engine::Instructions::Return, .argumentTypes = {}}},
        {FusionInstruction::GetField, FusionInstructionData{.instruction = Engine::Instructions::GetField, .argumentTypes = {InstructionArgumentType::FunctionName}}},
        {FusionInstruction::SetField, FusionInstructionData{.instruction = Engine::Instructions::SetField, .argumentTypes = {InstructionArgumentType::FunctionName}}},
        {FusionInstruction::GetConst, FusionInstructionData{.instruction = Engine::Instructions::GetConst, .argumentTypes = {InstructionArgumentType::MethodName}}},
        {FusionInstruction::HasField, FusionInstructionData{.instruction = Engine::Instructions::HasField, .argumentTypes = {}}},
        {FusionInstruction::CreateSoundPlayer, FusionInstructionData{.instruction = Engine::Instructions::CreateSoundPlayer, .argumentTypes = {}}},
        {FusionInstruction::CreateLabel, FusionInstructionData{.instruction = Engine::Instructions::CreateLabel, .argumentTypes = {}}},
        {FusionInstruction::PlaySound, FusionInstructionData{.instruction = Engine::Instructions::PlaySound, .argumentTypes = {}}},
        {FusionInstruction::GetSize, FusionInstructionData{.instruction = Engine::Instructions::GetSize, .argumentTypes = {}}},
        {FusionInstruction::SetSize, FusionInstructionData{.instruction = Engine::Instructions::SetSize, .argumentTypes = {}}},
        {FusionInstruction::AreOverlapping, FusionInstructionData{.instruction = Engine::Instructions::AreOverlapping, .argumentTypes = {}}},
        {FusionInstruction::Not, FusionInstructionData{.instruction = Engine::Instructions::Not, .argumentTypes = {}}},
        {FusionInstruction::ToString, FusionInstructionData{.instruction = Engine::Instructions::ToString, .argumentTypes = {}}},
        {FusionInstruction::ToInt, FusionInstructionData{.instruction = Engine::Instructions::ToInt, .argumentTypes = {}}},
        {FusionInstruction::ToFloat, FusionInstructionData{.instruction = Engine::Instructions::ToFloat, .argumentTypes = {}}},
        {FusionInstruction::GetGlobal, FusionInstructionData{.instruction = Engine::Instructions::GetGlobal, .argumentTypes = {InstructionArgumentType::FunctionName}}},
        {FusionInstruction::SetGlobal, FusionInstructionData{.instruction = Engine::Instructions::SetGlobal, .argumentTypes = {InstructionArgumentType::FunctionName}}},

    };
}