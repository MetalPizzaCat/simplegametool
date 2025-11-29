#include "MemoryObject.hpp"

void Engine::MemoryObject::increaseRefCounter()
{
    m_refCount++;
}