#pragma once
#include <cstdint>
namespace Engine
{
    using JumpDistanceType = int64_t;

    enum class Instructions
    {
        None,
        LoadConstString,
        CreateInstance,
        GetInstanceByName,
        PushInt,
        PushFloat,
        PushVector,
        PushTrue,
        PushFalse,
        SetLocal,
        GetLocal,
        Add,
        Sub,
        Div,
        Mul,
        And,
        Or,
        Not,
        // assign position of object on the stack to the value from the stack
        SetPosition,
        // push position of the object on the stack
        GetPosition,
        // create vector from two values on the stack
        MakeVector,
        GetVectorX,
        GetVectorY,
        Print,
        // Jump forwards or backwards by a given amount of bytes
        JumpBy,
        JumpByIf,
        Equals,
        NotEquals,
        More,
        Less,
        MoreOrEquals,
        LessOrEquals,
        // Call method of a given type by name
        CallMethod,
        // Call method of a given type by name, without passing "self" to method
        CallMethodStatic,
        // Call function of a scene
        CallFunction,
        // Exit function without returning a value
        ExitFunction,
        // Return value from a function and  exit
        Return,
        GetField,
        SetField,
        HasField,
        GetConst,
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
        ChangeScene,
        Destroy,
        IsDestroyed,
        Append,
        Length,
        CreateArray,
        GetItem,
        SetItem,
    };
}