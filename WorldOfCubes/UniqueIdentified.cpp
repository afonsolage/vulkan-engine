#include "stdafx.h"
#include "UniqueIdentified.h"
#include "Random.h"

void UniqueIdentified::generate_uid(const type_info & t_info)
{
	auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
	m_uid = now & std::numeric_limits<uint32_t>::max();
	m_uid |= Random::generate_uint();
}
