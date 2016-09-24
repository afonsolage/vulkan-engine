#pragma once

#define MAKE_VERSION(major, minor, patch) (((major) << 22) | ((minor) << 12) | (patch))
#define VERSION_MAJOR(version) ((uint32_t)(version) >> 22)
#define VERSION_MINOR(version) (((uint32_t)(version) >> 12) & 0x3ff)
#define VERSION_PATCH(version) ((uint32_t)(version) & 0xfff)

uint32_t clamp(uint32_t min, uint32_t max, uint32_t val);