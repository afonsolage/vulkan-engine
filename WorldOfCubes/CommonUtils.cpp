#include "stdafx.h"

uint32_t clamp(uint32_t min, uint32_t max, uint32_t val)
{
	return std::max(min, std::min(max, val));
}
