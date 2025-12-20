#pragma once
#include <cstdint>
#include <string>
#include <iostream>
namespace Engine
{
    /// @brief Base object for anything that need to be memory managed in some way
    class MemoryObject
    {
    public:
        explicit MemoryObject() = default;

        /// @brief Increase the counter for references pointing to this object
        void increaseRefCounter();

        /// @brief Decrease the counter for references pointing to this object
        void decreaseRefCounter();

        /// @brief Should this object be deleted by the garbage collector
        bool isDead() const { return m_dead || m_refCount == 0; }

        /// @brief Get string representation of this object, used for printing and such
        virtual std::string toString() const = 0;

        virtual ~MemoryObject() {}

    private:
        bool m_dead = false;
        int32_t m_refCount = 0;
    };

    /// @brief Wrapper around native string that can be stored in the managed memory of the engine
    class StringObject : public MemoryObject
    {
    public:
        explicit StringObject(std::string const &str) : m_string(str) {}

        std::string &getString() { return m_string; }

        std::string toString() const override { return m_string; }

    private:
        std::string m_string;
    };
}