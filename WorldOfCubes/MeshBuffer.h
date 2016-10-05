#pragma once

#include "VulkanAPI.h"
#include <glm/glm.hpp>

enum class MeshBufferElementFormat
{
	eFloat,
	eUnsignedInt,
	eSignedInt,
};

enum class MeshBufferElementBits
{
	e8 = 1,
	e16 = 2,
	e32 = 4,
	e64 = 8,
};

enum class MeshBufferElementCount
{
	e1 = 1,
	e2 = 2,
	e3 = 3,
	e4 = 4,
};

class MeshBuffer
{
private:
	struct MeshBufferElement
	{
		MeshBufferElement(uint32_t location, MeshBufferElementFormat format, MeshBufferElementBits element_bytes, MeshBufferElementCount element_count);

		const uint32_t m_location;
		const MeshBufferElementFormat m_format;
		const MeshBufferElementBits m_element_bits;
		const MeshBufferElementCount m_element_count;
		const size_t m_element_size;
		size_t m_offset;
	};
public:
	MeshBuffer();
	virtual ~MeshBuffer();

	uint32_t add_buffer_element(MeshBufferElementFormat format, MeshBufferElementBits element_size, MeshBufferElementCount element_count);

	template<typename T>
	uint32_t set_buffer(uint32_t location, const std::vector<T>& buffer);

	void initialize();

	const std::vector<char>& get_buffer() const { return m_buffer; }
private:

	const MeshBufferElement& get_element(uint32_t location);

	bool m_initialized;
	std::vector<MeshBufferElement> m_buffer_elements;
	std::vector<char> m_buffer;

	size_t m_per_vertex_size;
#ifndef NDEBUG
	size_t m_vertex_count;
#endif
};

template<typename T>
uint32_t MeshBuffer::set_buffer(uint32_t location, const std::vector<T>& buffer)
{
	if (!m_initialized)
	{
		LOG_ERROR("Can't set buffer elements before initialization.");
		return std::numeric_limits<uint32_t>::max();
	}

	auto element = get_element(location);
	auto element_bytes = (size_t)element.m_element_bits;

	if (element_bytes != sizeof(T))
	{
		LOG_ERROR("Invalid buffer type. Expected type bytes: %d, received: %d.", element_bytes, sizeof(T));
		throw std::invalid_argument("Element type and buffer type missmatch!");
	}

	auto element_count = (size_t)element.m_element_count;

	if (buffer.size() % element_count != 0)
	{
		LOG_ERROR("Can't use the provided buffer! Element count: %d, buffer size: %d", element_count, buffer.size());
		throw std::invalid_argument("Element count and buffer size missmatch!");
	}

	uint32_t vertex_count = buffer.size() / element_count;

#ifndef NDEBUG
	if (m_vertex_count == 0)
	{
		m_vertex_count = vertex_count;
	}
	else if (m_vertex_count != vertex_count)
	{
		LOG_DEBUG("Vertex count missmatch between set buffer. Last vertex count: %d, current vertex count: %d.", m_vertex_count, vertex_count);
	}
#endif
	m_buffer.resize(vertex_count * m_per_vertex_size);

	for (uint32_t i = 0; i < vertex_count; i++)
	{
		memcpy(&m_buffer[(i*m_per_vertex_size) + element.m_offset], &buffer[i*element_count], element.m_element_size);
	}

	return vertex_count;
}