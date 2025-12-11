#include "FunctionRef.hpp"

Engine::FunctionRef::FunctionRef(Runnable::RunnableFunction const *func, GameObject *inst) : m_func(func), m_instance(inst), m_isNative(false)
{
}

Engine::FunctionRef::FunctionRef(std::function<void(Scene &scene)>  const& func) : m_nativeFunc(func), m_isNative(false)
{
}
