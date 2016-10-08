#include "stdafx.h"
#include "MeshBuffer.h"

MeshBuffer::MeshBuffer()
	: m_per_vertex_size(0)
	, m_initialized(false)
#ifndef NDEBUG
	, m_vertex_count(0)
#endif
{
}

MeshBuffer::MeshBuffer(const MeshBuffer & other)
	: m_initialized(true)
	, m_per_vertex_size(other.m_per_vertex_size)
	, m_buffer_elements(other.m_buffer_elements)
#ifndef NDEBUG
	, m_vertex_count(0)
#endif
{

}


MeshBuffer::~MeshBuffer()
{
}

uint32_t MeshBuffer::add_buffer_element(MeshBufferElementFormat format, MeshBufferElementBits element_size, MeshBufferElementCount element_count)
{
	if (m_initialized)
	{
		LOG_ERROR("Can't add buffer elements after initialized.");
		return std::numeric_limits<uint32_t>::max();
	}

	auto m_location = (uint32_t)m_buffer_elements.size();
	m_buffer_elements.emplace_back(m_location, format, element_size, element_count);
	return m_location;
}

void MeshBuffer::initialize()
{
	m_initialized = true;
	m_per_vertex_size = 0;

	for (auto& element : m_buffer_elements)
	{
		element.m_offset = m_per_vertex_size;
		m_per_vertex_size += element.m_element_size;
	}
}

void MeshBuffer::set_indexes(const std::vector<uint16_t>& buffer)
{
#ifndef NDEBUG
	if (m_vertex_count > 0 && buffer.size() != m_vertex_count)
	{
		LOG_DEBUG("Vertex count missmatch between set buffer and set indexes. Last vertex count: %d, current vertex count: %d.", m_vertex_count, buffer.size());
	}
#endif
	m_indexes.resize(buffer.size());
	memcpy(&m_indexes[0], &buffer[0], buffer.size() * sizeof(uint16_t));
}

const MeshBuffer::MeshBufferElement& MeshBuffer::get_element(uint32_t location)
{
	auto it = std::find_if(std::begin(m_buffer_elements), std::end(m_buffer_elements), [&location](const auto& element)
	{
		return element.m_location == location;
	});

	if (it == m_buffer_elements.end())
	{
		LOG_FATAL("MeshBuffer element %d not found!", location);
		throw std::invalid_argument("Mesh buffer element does not exists!");
	}
	else
	{
		return *it;
	}
}

MeshBuffer::MeshBufferElement::MeshBufferElement(uint32_t location, MeshBufferElementFormat format, MeshBufferElementBits element_bits, MeshBufferElementCount element_count)
	: m_location(location)
	, m_format(format)
	, m_element_bits(element_bits)
	, m_element_count(element_count)
	, m_element_size((sizeof(char) * ((size_t)m_element_bits) * ((size_t)element_count)))
{

}
