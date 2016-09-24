#include "stdafx.h"
#include "GraphicsPipeline.h"
#include "GraphicsSystem.h"
#include "Swapchain.h"
#include "Context.h"
#include "ShaderManager.h"
#include "MeshObject.h"

GraphicsPipeline::GraphicsPipeline(std::shared_ptr<GraphicsSystem>& graphics_system)
	: m_graphics_system(graphics_system)
{
	m_context = graphics_system->get_context();
	m_swapchain = graphics_system->get_swapchain();
}


GraphicsPipeline::~GraphicsPipeline()
{
	GET_CONTEXT;

	context->m_device.destroyDescriptorSetLayout(m_descriptor_set_layout);
	context->m_device.destroyRenderPass(m_render_pass);
	context->m_device.destroyPipelineLayout(m_pipeline_layout);
	context->m_device.destroyPipeline(m_pipeline);

	LOG_DEBUG("Graphics pipeline destroyed.");
}

void GraphicsPipeline::init()
{
	create_render_pass();
	create_descriptor_set_layout();
	create_pipeline_layout();
	create_pipeline();
}

void GraphicsPipeline::create_render_pass()
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

void GraphicsPipeline::create_descriptor_set_layout()
{
	GET_CONTEXT;

	vk::DescriptorSetLayoutBinding ds_ubo_layout_binding;
	ds_ubo_layout_binding.binding = 0;
	ds_ubo_layout_binding.descriptorCount = 1;
	ds_ubo_layout_binding.descriptorType = vk::DescriptorType::eUniformBuffer;
	ds_ubo_layout_binding.stageFlags = vk::ShaderStageFlagBits::eVertex;

	vk::DescriptorSetLayoutBinding ds_sampler_layout_binding;
	ds_sampler_layout_binding.binding = 1;
	ds_sampler_layout_binding.descriptorCount = 1;
	ds_sampler_layout_binding.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	ds_sampler_layout_binding.stageFlags = vk::ShaderStageFlagBits::eFragment;

	std::array<vk::DescriptorSetLayoutBinding, 2> bindings { ds_ubo_layout_binding, ds_sampler_layout_binding };

	vk::DescriptorSetLayoutCreateInfo ds_layout_create_info;
	ds_layout_create_info.bindingCount = 2;
	ds_layout_create_info.pBindings = bindings.data();

	m_descriptor_set_layout = context->m_device.createDescriptorSetLayout(ds_layout_create_info);
}

void GraphicsPipeline::create_pipeline_layout()
{
	vk::PipelineLayoutCreateInfo create_info;

	create_info.setLayoutCount = 1;
	create_info.pSetLayouts = &m_descriptor_set_layout;

	GET_CONTEXT;

	m_pipeline_layout = context->m_device.createPipelineLayout(create_info);
}

void GraphicsPipeline::create_pipeline()
{
	GET_CONTEXT;
	GET_SWAPCHAIN;

	SAFE_GET(graphics_system, m_graphics_system);
	SAFE_GET(shader_manager, graphics_system->get_shader_manager());

	//SHADER STAGES
	auto shader_stages = shader_manager->get_shader_create_info({ ShaderManager::Shader::eVert, ShaderManager::Shader::eFrag });


	//VERTEX INPUT STAGE
	auto binding_description = MeshObject::get_vertex_input_binding_description();
	auto attribute_description = MeshObject::get_vertex_input_attribute_description();

	vk::PipelineVertexInputStateCreateInfo vertex_input_stage;
	vertex_input_stage.vertexBindingDescriptionCount = 1;
	vertex_input_stage.pVertexBindingDescriptions = &binding_description;
	vertex_input_stage.vertexAttributeDescriptionCount = attribute_description.size();
	vertex_input_stage.pVertexAttributeDescriptions = attribute_description.data();


	//INPUT ASSEMBLY STAGE
	vk::PipelineInputAssemblyStateCreateInfo input_assembly_stage;
	input_assembly_stage.topology = vk::PrimitiveTopology::eTriangleList;
	input_assembly_stage.primitiveRestartEnable = VK_FALSE;


	//RASTERIZATION STAGE
	vk::PipelineRasterizationStateCreateInfo rasterization_stage;
	rasterization_stage.depthClampEnable = VK_FALSE;
	rasterization_stage.rasterizerDiscardEnable = VK_FALSE;
	rasterization_stage.polygonMode = vk::PolygonMode::eFill;
	rasterization_stage.lineWidth = 1.0f;
	rasterization_stage.cullMode = vk::CullModeFlagBits::eBack;
	rasterization_stage.frontFace = vk::FrontFace::eCounterClockwise;
	rasterization_stage.depthBiasEnable = VK_FALSE;


	//COLOR BLEND STAGE
	vk::PipelineColorBlendAttachmentState blend_attachment;

	blend_attachment.blendEnable = VK_FALSE;
	blend_attachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;

	vk::PipelineColorBlendStateCreateInfo color_blend_stage;
	color_blend_stage.attachmentCount = 1;
	color_blend_stage.pAttachments = &blend_attachment;


	//VIEWPORT STAGE
	vk::PipelineViewportStateCreateInfo viewport_stage;

	vk::Viewport viewport = { 0,0,(float)swapchain->m_extent.width,(float)swapchain->m_extent.height,0.0f,1.0f };
	vk::Rect2D scissor = { { 0, 0 }, swapchain->m_extent };

	viewport_stage.viewportCount = 1;
	viewport_stage.pViewports = &viewport;
	viewport_stage.scissorCount = 1;
	viewport_stage.pScissors = &scissor;


	//DEPTH STENCIL STAGE
	vk::PipelineDepthStencilStateCreateInfo depth_stencil_stage;
	depth_stencil_stage.depthTestEnable = VK_TRUE;
	depth_stencil_stage.depthWriteEnable = VK_TRUE;
	depth_stencil_stage.depthCompareOp = vk::CompareOp::eLess;
	depth_stencil_stage.depthBoundsTestEnable = VK_FALSE;
	depth_stencil_stage.stencilTestEnable = VK_FALSE;


	//MULTISAMPLE STAGE
	vk::PipelineMultisampleStateCreateInfo multisample_stage;
	multisample_stage.rasterizationSamples = vk::SampleCountFlagBits::e1;


	//GRAPHICS PIPELINE CREATION
	vk::GraphicsPipelineCreateInfo graphics_pipeline_create_info;

	graphics_pipeline_create_info.stageCount = shader_stages.size();
	graphics_pipeline_create_info.pStages = shader_stages.data();
	graphics_pipeline_create_info.pDynamicState = VK_NULL_HANDLE;
	graphics_pipeline_create_info.pVertexInputState = &vertex_input_stage;
	graphics_pipeline_create_info.pInputAssemblyState = &input_assembly_stage;
	graphics_pipeline_create_info.pRasterizationState = &rasterization_stage;
	graphics_pipeline_create_info.pColorBlendState = &color_blend_stage;
	graphics_pipeline_create_info.pViewportState = &viewport_stage;
	graphics_pipeline_create_info.pDepthStencilState = &depth_stencil_stage;
	graphics_pipeline_create_info.pMultisampleState = &multisample_stage;
	graphics_pipeline_create_info.layout = m_pipeline_layout;
	graphics_pipeline_create_info.renderPass = m_render_pass;
	graphics_pipeline_create_info.subpass = 0;
	graphics_pipeline_create_info.basePipelineHandle = vk::Pipeline();
	graphics_pipeline_create_info.basePipelineIndex = -1;

	m_pipeline = context->m_device.createGraphicsPipeline(vk::PipelineCache(), graphics_pipeline_create_info);
}

vk::AttachmentDescription GraphicsPipeline::get_depth_attachment() const
{
	GET_CONTEXT;
	vk::Format depth_format = context->find_supported_format({ vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
		vk::ImageTiling::eOptimal, vk::FormatFeatureFlagBits::eDepthStencilAttachment);

	vk::AttachmentDescription description;

	description.format = depth_format;
	description.samples = vk::SampleCountFlagBits::e1; //TODO: Add settings for sampling
	description.loadOp = vk::AttachmentLoadOp::eClear;
	description.storeOp = vk::AttachmentStoreOp::eDontCare;
	description.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	description.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	description.initialLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
	description.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

	return description;
}
