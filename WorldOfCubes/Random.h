#pragma once
#include <random>

class Random
{
public:
	static uint32_t generate_uint();
private:
	static std::random_device m_rand_device;
	static std::default_random_engine m_rand_engine;

	static std::uniform_int_distribution<uint32_t> m_uint_distribution;

	Random() = delete;
	virtual ~Random() = delete;
};

