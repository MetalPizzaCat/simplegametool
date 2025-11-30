#pragma once

namespace Engine
{
enum class Instructions
{
    None,
    LoadConstString,
    CreateInstance,
    PushInt,
    PushFloat,
    PushVector,
    SetLocal,
    GetLocal,
    Add,
    Sub,
    Div,
    Mul,
    And,
    Or,
    Call,
    Return,
    // assign position of object on the stack to the value from the stack
    SetPosition,
    // push position of the object on the stack
    GetPosition,
    // create vector from two values on the stack
    MakeVector,
    GetVectorX,
    GetVectorY,
    Print,
};
}