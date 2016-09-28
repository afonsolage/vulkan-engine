#pragma once

#include "VulkanAPI.h"

class GraphicsSystem;
class Context;

class ColoredMaterial
{
public:

	ColoredMaterial(std::shared_ptr<GraphicsSystem> graphics_system);
	virtual ~ColoredMaterial();

	vk::Pipeline get_pipeline() const { return m_base_pipeline; }

private:

	void create_descriptor_set_layout();
	void create_pipeline_layout();
	void create_pipeline();

	std::weak_ptr<Context> m_context;
	std::weak_ptr<GraphicsSystem> m_graphics_system;

	vk::DescriptorSetLayout m_descriptor_set_layout;
	vk::PipelineLayout m_pipeline_layout;
	vk::Pipeline m_base_pipeline;
};

