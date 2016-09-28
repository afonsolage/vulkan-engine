#include "stdafx.h"
#include "MeshObject.h"


MeshObject::MeshObject()
{
}


MeshObject::~MeshObject()
{
}

vk::VertexInputBindingDescription MeshObject::get_vertex_input_binding_description() noexcept
{
	vk::VertexInputBindingDescription binding_description;

	binding_description.binding = 0;
	binding_description.stride = sizeof(VertexData);
	binding_description.inputRate = vk::VertexInputRate::eVertex;

	return binding_description;
}

VertexDataAttributeDescriptions MeshObject::get_vertex_input_attribute_description() noexcept
{
	VertexDataAttributeDescriptions attribute_descriptions;

	attribute_descriptions[0].binding = 0;
	attribute_descriptions[0].location = 0;
	attribute_descriptions[0].format = vk::Format::eR32G32B32Sfloat;
	attribute_descriptions[0].offset = offsetof(VertexData, pos);

	attribute_descriptions[1].binding = 0;
	attribute_descriptions[1].location = 1;
	attribute_descriptions[1].format = vk::Format::eR32G32B32Sfloat;
	attribute_descriptions[1].offset = offsetof(VertexData, color);

	//attribute_descriptions[2].binding = 0;
	//attribute_descriptions[2].location = 2;
	//attribute_descriptions[2].format = vk::Format::eR32G32Sfloat;
	//attribute_descriptions[2].offset = offsetof(VertexData, tex_coord);
	
	return attribute_descriptions;
}
