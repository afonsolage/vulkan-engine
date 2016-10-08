#pragma once

#include "VulkanAPI.h"
#include "MeshBuffer.h"


class GraphicsSystem;
class Context;
class ShaderSystem;
class MeshComponent;
class CameraComponent;

struct ShaderInfo;

class AbstractMaterial
{
public:
	AbstractMaterial(std::shared_ptr<GraphicsSystem> graphics_system);
	virtual ~AbstractMaterial();

	MeshBuffer create_mesh_buffer() const;
	vk::Pipeline get_pipeline() const { return m_base_pipeline; }

	virtual void init();

	virtual void set_view(glm::mat4& view) {};
	virtual void set_projection(glm::mat4& view) {};

	virtual void pre_render(std::shared_ptr<CameraComponent> component) {};
	virtual void render(std::shared_ptr<MeshComponent> component) {};

protected:
	virtual void create_descriptor_set_layout() = 0;
	virtual void create_pipeline_layout();
	virtual void create_pipeline();

	vk::DescriptorSetLayout m_descriptor_set_layout;
	vk::PipelineLayout m_pipeline_layout;
	vk::Pipeline m_base_pipeline;

	std::weak_ptr<Context> m_context;
	std::weak_ptr<GraphicsSystem> m_graphics_system;
	std::weak_ptr<ShaderSystem> m_shader_system;

	std::weak_ptr<ShaderInfo> m_vertex_shader;
	std::weak_ptr<ShaderInfo> m_fragment_shader;
};

