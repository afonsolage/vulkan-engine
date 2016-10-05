#pragma once

#include "VulkanAPI.h"
#include "MeshBuffer.h"


class GraphicsSystem;
class Context;
class ShaderSystem;

struct ShaderInfo;

class AbstractMaterial
{
public:
	AbstractMaterial(std::shared_ptr<GraphicsSystem> graphics_system);
	virtual ~AbstractMaterial();

	MeshBuffer create_mesh_buffer() const;
	vk::Pipeline get_pipeline() const { return m_base_pipeline; }

protected:
	void create_descriptor_set_layout();
	void create_pipeline_layout();
	void create_pipeline();

	vk::DescriptorSetLayout m_descriptor_set_layout;
	vk::PipelineLayout m_pipeline_layout;
	vk::Pipeline m_base_pipeline;

	std::weak_ptr<Context> m_context;
	std::weak_ptr<GraphicsSystem> m_graphics_system;
	std::weak_ptr<ShaderSystem> m_shader_system;

	std::weak_ptr<ShaderInfo> m_vertex_shader;
	std::weak_ptr<ShaderInfo> m_fragment_shader;
};

