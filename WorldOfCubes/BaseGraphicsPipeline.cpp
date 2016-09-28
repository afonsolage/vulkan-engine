#include "stdafx.h"
#include "BaseGraphicsPipeline.h"
#include "GraphicsSystem.h"
#include "Swapchain.h"
#include "Context.h"
#include "ShaderSystem.h"
#include "MeshObject.h"

BaseGraphicsPipeline::BaseGraphicsPipeline(std::shared_ptr<GraphicsSystem>& graphics_system)
	: m_graphics_system(graphics_system)
{
	m_context = graphics_system->get_context();
	m_swapchain = graphics_system->get_swapchain();
}


BaseGraphicsPipeline::~BaseGraphicsPipeline()
{
	GET_CONTEXT;

	context->m_device.destroyImageView(m_depth_image_view);
	context->m_device.destroyImage(m_depth_image);
	context->m_device.freeMemory(m_depth_memory);

	context->m_device.destroyRenderPass(m_render_pass);
}

void BaseGraphicsPipeline::init()
{
	m_blend_attachment_opaque_rgba.blendEnable = VK_FALSE;
	m_blend_attachment_opaque_rgba.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;

	GET_SWAPCHAIN;

	m_viewport = { 0,0,(float)swapchain->m_extent.width,(float)swapchain->m_extent.height,0.0f,1.0f };
	m_scissor = { { 0, 0 }, swapchain->m_extent };

	GET_CONTEXT;
	m_depth_format = context->find_supported_format({ vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
		vk::ImageTiling::eOptimal, vk::FormatFeatureFlagBits::eDepthStencilAttachment);

	create_render_pass();
	create_depth_buffer();
}

vk::PipelineInputAssemblyStateCreateInfo BaseGraphicsPipeline::get_input_assembly_stage_create_info()
{
	vk::PipelineInputAssemblyStateCreateInfo input_assembly_stage;
	input_assembly_stage.topology = vk::PrimitiveTopology::eTriangleList;
	input_assembly_stage.primitiveRestartEnable = VK_FALSE;

	return input_assembly_stage;
}

vk::PipelineRasterizationStateCreateInfo BaseGraphicsPipeline::get_rasterization_stage_create_info()
{
	vk::PipelineRasterizationStateCreateInfo rasterization_stage;
	rasterization_stage.depthClampEnable = VK_FALSE;
	rasterization_stage.rasterizerDiscardEnable = VK_FALSE;
	rasterization_stage.polygonMode = vk::PolygonMode::eFill;
	rasterization_stage.lineWidth = 1.0f;
	rasterization_stage.cullMode = vk::CullModeFlagBits::eBack;
	rasterization_stage.frontFace = vk::FrontFace::eCounterClockwise;
	rasterization_stage.depthBiasEnable = VK_FALSE;

	return rasterization_stage;
}

vk::PipelineColorBlendStateCreateInfo BaseGraphicsPipeline::get_color_blend_stage_create_info()
{
	vk::PipelineColorBlendStateCreateInfo color_blend_stage;
	color_blend_stage.attachmentCount = 1;
	color_blend_stage.pAttachments = &m_blend_attachment_opaque_rgba;
	return color_blend_stage;
}

vk::PipelineViewportStateCreateInfo BaseGraphicsPipeline::get_viewport_stage_create_info()
{
	vk::PipelineViewportStateCreateInfo viewport_stage;
	viewport_stage.viewportCount = 1;
	viewport_stage.pViewports = &m_viewport;
	viewport_stage.scissorCount = 1;
	viewport_stage.pScissors = &m_scissor;
	return viewport_stage;
}

vk::PipelineDepthStencilStateCreateInfo BaseGraphicsPipeline::get_depth_stencil_stage_create_info()
{
	vk::PipelineDepthStencilStateCreateInfo depth_stencil_stage;
	depth_stencil_stage.depthTestEnable = VK_TRUE;
	depth_stencil_stage.depthWriteEnable = VK_TRUE;
	depth_stencil_stage.depthCompareOp = vk::CompareOp::eLess;
	depth_stencil_stage.depthBoundsTestEnable = VK_FALSE;
	depth_stencil_stage.stencilTestEnable = VK_FALSE;
	return depth_stencil_stage;
}

vk::PipelineMultisampleStateCreateInfo BaseGraphicsPipeline::get_multisample_stage_create_info()
{
	vk::PipelineMultisampleStateCreateInfo multisample_stage;
	multisample_stage.rasterizationSamples = vk::SampleCountFlagBits::e1;
	return multisample_stage;
}

void BaseGraphicsPipeline::create_render_pass()
{
	GET_SWAPCHAIN;

	auto swapchain_attachment = swapchain->get_attachment_description();
	auto depth_attachment = get_depth_attachment();

	vk::AttachmentReference swapchain_ref = { 0, vk::ImageLayout::eColorAttachmentOptimal };
	vk::AttachmentReference depth_ref = { 1, vk::ImageLayout::eDepthStencilAttachmentOptimal };

	vk::SubpassDescription subpass_description;
	subpass_description.colorAttachmentCount = 1;
	subpass_description.pColorAttachments = &swapchain_ref;
	subpass_description.pDepthStencilAttachment = &depth_ref;

	vk::AttachmentDescription attachments[] = { swapchain_attachment, depth_attachment };

	vk::RenderPassCreateInfo render_pass_info;
	render_pass_info.attachmentCount = 2;
	render_pass_info.pAttachments = attachments;
	render_pass_info.subpassCount = 1;
	render_pass_info.pSubpasses = &subpass_description;

	GET_CONTEXT;
	m_render_pass = context->m_device.createRenderPass(render_pass_info);
}

void BaseGraphicsPipeline::create_depth_buffer()
{
	GET_CONTEXT;
	GET_SWAPCHAIN;

	m_depth_format = context->find_supported_format({ vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
		vk::ImageTiling::eOptimal, vk::FormatFeatureFlagBits::eDepthStencilAttachment);

	context->create_image({ (uint32_t)m_viewport.width, (uint32_t)m_viewport.height, (uint32_t)m_viewport.maxDepth },
		vk::ImageTiling::eOptimal, m_depth_format,
		vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal, m_depth_image, m_depth_memory);
	m_depth_image_view = context->create_image_view(m_depth_image, m_depth_format, vk::ImageAspectFlagBits::eDepth);
}

vk::AttachmentDescription BaseGraphicsPipeline::get_depth_attachment() const
{
	vk::AttachmentDescription description;

	description.format = m_depth_format;
	description.samples = vk::SampleCountFlagBits::e1; //TODO: Add settings for sampling
	description.loadOp = vk::AttachmentLoadOp::eClear;
	description.storeOp = vk::AttachmentStoreOp::eDontCare;
	description.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	description.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	description.initialLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
	description.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

	return description;
}
