#include "Random.hpp"

void Engine::Random::seed(uint64_t seed)
{
    m_rng.seed(seed);
}