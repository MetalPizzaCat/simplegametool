#pragma once
#include <random>

namespace Engine
{
    class Random
    {
    public:
        /// @brief Get current global instance of the random generator
        /// @return
        static Random &getInstance()
        {
            static Random random;
            return random;
        }

        /// @brief Set the random generator with a given value
        /// @param seed
        void seed(uint64_t seed);

        /// @brief Get random integer value in range from min(inclusive) to max(exclusive) using uniform distribution
        /// @tparam T Type of the value to return
        /// @param min Min inclusive value
        /// @param max Max inclusive value
        /// @return
        template <typename T>
        T getRandomIntInRange(T min, T max)
        {
            std::uniform_int_distribution<T> dist(min, max);
            return dist(m_rng);
        }

        /// @brief Get random floating point value in range from min(inclusive) to max(exclusive) using uniform distribution
        /// @tparam T Type of the value to return
        /// @param min Min inclusive value
        /// @param max Max inclusive value
        /// @return
        template <typename T>
        T getRandomFloatInRange(T min, T max)
        {
            std::uniform_real_distribution<T> dist(min, max);
            return dist(m_rng);
        }

    private:
        std::mt19937_64 m_rng;
    };
} // namespace Engine
