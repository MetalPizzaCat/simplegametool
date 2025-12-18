#include "MemoryObject.hpp"

void Engine::MemoryObject::increaseRefCounter()
{
    m_refCount++;
}

void Engine::MemoryObject::decreaseRefCounter()
{
    m_refCount--;
}