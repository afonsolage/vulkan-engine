#include "stdafx.h"
#include "Random.h"

std::random_device Random::m_rand_device;
std::default_random_engine Random::m_rand_engine(m_rand_device());
std::uniform_int_distribution<uint32_t> Random::m_uint_distribution;

uint32_t Random::generate_uint()
{
	return m_uint_distribution(m_rand_engine);
}
