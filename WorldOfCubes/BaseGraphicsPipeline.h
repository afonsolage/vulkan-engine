#pragma once

#include "VulkanAPI.h"

//Forwarding declarations;
class GraphicsSystem;
class Context;
class Swapchain;

class BaseGraphicsPipeline
{
public:
	BaseGraphicsPipeline(std::shared_ptr<GraphicsSystem>& graphics_system);
	virtual ~BaseGraphicsPipeline();

	void init();

	vk::PipelineInputAssemblyStateCreateInfo get_input_assembly_stage_create_info();
	vk::PipelineRasterizationStateCreateInfo get_rasterization_stage_create_info();
	vk::PipelineColorBlendStateCreateInfo get_color_blend_stage_create_info();
	vk::PipelineViewportStateCreateInfo get_viewport_stage_create_info();
	vk::PipelineDepthStencilStateCreateInfo get_depth_stencil_stage_create_info();
	vk::PipelineMultisampleStateCreateInfo get_multisample_stage_create_info();

	vk::RenderPass get_render_pass() const noexcept { return m_render_pass; }

private:
	void create_render_pass();
	void create_depth_buffer();

	vk::AttachmentDescription get_depth_attachment() const;

	vk::RenderPass m_render_pass;
	vk::Image m_depth_image;
	vk::ImageView m_depth_image_view;
	vk::DeviceMemory m_depth_memory;

	std::weak_ptr<GraphicsSystem> m_graphics_system;
	std::weak_ptr<Context> m_context;
	std::weak_ptr<Swapchain> m_swapchain;

	vk::Viewport m_viewport;
	vk::Rect2D m_scissor;
	vk::Format m_depth_format;

	vk::PipelineColorBlendAttachmentState m_blend_attachment_opaque_rgba;
};

