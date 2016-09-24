#pragma once

#include "VulkanAPI.h"
#include <glm/glm.hpp>

struct VertexData
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 tex_coord;
};

class MeshObject
{
public:
	MeshObject();
	virtual ~MeshObject();

	static vk::VertexInputBindingDescription get_vertex_input_binding_description() noexcept;
	static std::vector<vk::VertexInputAttributeDescription> get_vertex_input_attribute_description() noexcept;

private:

	static const int POS = 0;
	static const int COLOR = 1;
	static const int TEX_COLOR = 2;
};

