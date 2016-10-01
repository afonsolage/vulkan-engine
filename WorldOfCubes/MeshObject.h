#pragma once

#include "VulkanAPI.h"
#include <glm/glm.hpp>

struct VertexData
{
	glm::vec3 pos;
	glm::vec3 color;
	//glm::vec2 tex_coord;
};

using VertexDataAttributeDescriptions = std::array<vk::VertexInputAttributeDescription, 2>;

class MeshObject
{
public:
	MeshObject();
	virtual ~MeshObject();

	static vk::VertexInputBindingDescription get_vertex_input_binding_description() noexcept;

	static VertexDataAttributeDescriptions get_vertex_input_attribute_description() noexcept;

private:

	vk::Buffer m_vertex_buffer;

	static const int POS = 0;
	static const int COLOR = 1;
	//static const int TEX_COLOR = 2;
};

