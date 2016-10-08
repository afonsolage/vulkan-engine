#pragma once

#define MAKE_VERSION(major, minor, patch) (((major) << 22) | ((minor) << 12) | (patch))
#define VERSION_MAJOR(version) ((uint32_t)(version) >> 22)
#define VERSION_MINOR(version) (((uint32_t)(version) >> 12) & 0x3ff)
#define VERSION_PATCH(version) ((uint32_t)(version) & 0xfff)

uint32_t clamp(uint32_t min, uint32_t max, uint32_t val);

inline void split(const std::string& str, char delim, std::vector<std::string>& result)
{
	std::stringstream ss;
	ss.str(str);
	std::string element;
	while (std::getline(ss, element, delim))
	{
		if (element.empty())
		{
			continue;
		}

		result.push_back(element);
	}
}

template<typename T1, typename T2>
inline void convert_buffer(const std::vector<T1>& from, std::vector<T2>& to)
{
	auto bytes_to_read = sizeof(T1) * from.size();
	auto dest_size = bytes_to_read / sizeof(T2);

	to.resize(dest_size);

	memcpy(&to[0], &from[0], bytes_to_read);
}