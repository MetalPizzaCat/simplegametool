#pragma once
#include <functional>
#include "Runnable.hpp"
#include "GameObject.hpp"
#include "MemoryObject.hpp"
namespace Engine
{
    class FunctionRef : public MemoryObject
    {
    public:
        explicit FunctionRef(Runnable::RunnableFunction const *func, GameObject *inst);
        explicit FunctionRef(std::function<void(Scene &scene)> const &func);

        bool isNative() const { return m_isNative; }

        Runnable::RunnableFunction const *getRunnableFunction() const { return m_func; }

    private:
        bool m_isNative;
        Runnable::RunnableFunction const *m_func;
        GameObject *m_instance;
        std::function<void(Scene &scene)> m_nativeFunc;
    };
} // namespace Engine
