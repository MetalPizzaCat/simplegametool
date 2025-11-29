#pragma once
#include <cstdint>
#include <string>
namespace Engine
{
    class MemoryObject
    {
    public:
        explicit MemoryObject() = default;
        void increaseRefCounter();
        bool isDead() const { return m_dead; }
        virtual ~MemoryObject() {}

    private:
        bool m_dead;
        int32_t m_refCount;
    };

    class StringObject : public MemoryObject
    {
    public:
        explicit StringObject(std::string const &str) : m_string(str) {}

        std::string const &getString() const { return m_string; }

    private:
        std::string m_string;
    };
}