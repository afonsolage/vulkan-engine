#pragma once

#include "VulkanAPI.h"

//Forwarding declarations;
class GraphicsSystem;
class Context;
class Swapchain;

class GraphicsPipeline
{
public:
	GraphicsPipeline(std::shared_ptr<GraphicsSystem>& graphics_system);
	virtual ~GraphicsPipeline();

	void init();

private:
	void create_render_pass();
	void create_descriptor_set_layout();
	void create_pipeline_layout();
	void create_pipeline();

	vk::AttachmentDescription get_depth_attachment() const;

	vk::RenderPass m_render_pass;
	vk::DescriptorSetLayout m_descriptor_set_layout;
	vk::PipelineLayout m_pipeline_layout;
	vk::Pipeline m_pipeline;

	std::weak_ptr<GraphicsSystem> m_graphics_system;
	std::weak_ptr<Context> m_context;
	std::weak_ptr<Swapchain> m_swapchain;
};

