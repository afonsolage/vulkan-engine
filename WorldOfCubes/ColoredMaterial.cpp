#include "stdafx.h"
#include "ColoredMaterial.h"
#include "GraphicsSystem.h"
#include "Context.h"
#include "ShaderSystem.h"
#include "BaseGraphicsPipeline.h"

ColoredMaterial::ColoredMaterial(std::shared_ptr<GraphicsSystem> graphics_system)
	: m_graphics_system(graphics_system)
	, m_context(graphics_system->get_context())
{
	create_descriptor_set_layout();
	create_pipeline_layout();
	create_pipeline();
}


ColoredMaterial::~ColoredMaterial()
{
	GET_CONTEXT;

	context->m_device.destroyDescriptorSetLayout(m_descriptor_set_layout);
	context->m_device.destroyPipelineLayout(m_pipeline_layout);
	context->m_device.destroyPipeline(m_base_pipeline);

	LOG_DEBUG("Colored Material destroyed.");
}

void ColoredMaterial::create_descriptor_set_layout()
{
	GET_CONTEXT;

	vk::DescriptorSetLayoutBinding ds_ubo_layout_binding;
	ds_ubo_layout_binding.binding = 0;
	ds_ubo_layout_binding.descriptorCount = 1;
	ds_ubo_layout_binding.descriptorType = vk::DescriptorType::eUniformBuffer;
	ds_ubo_layout_binding.stageFlags = vk::ShaderStageFlagBits::eVertex;

	vk::DescriptorSetLayoutCreateInfo ds_layout_create_info;
	ds_layout_create_info.bindingCount = 1;
	ds_layout_create_info.pBindings = &ds_ubo_layout_binding;

	m_descriptor_set_layout = context->m_device.createDescriptorSetLayout(ds_layout_create_info);
}

void ColoredMaterial::create_pipeline_layout()
{
	vk::PipelineLayoutCreateInfo create_info;

	create_info.setLayoutCount = 1;
	create_info.pSetLayouts = &m_descriptor_set_layout;

	GET_CONTEXT;

	m_pipeline_layout = context->m_device.createPipelineLayout(create_info);
}

void ColoredMaterial::create_pipeline()
{
	SAFE_GET(graphics_system, m_graphics_system);
	SAFE_GET(shader_system, graphics_system->get_shader_system());

	//SHADER STAGES
	auto shader_stages = shader_system->get_shader_create_info({ ShaderSystem::Shader::eColoredVert, ShaderSystem::Shader::eColoredFrag });

	//VERTEX INPUT STAGE
	/*auto binding_description = MeshBuffer::get_vertex_input_binding_description();
	auto attribute_description = MeshBuffer::get_vertex_input_attribute_description();*/

	vk::PipelineVertexInputStateCreateInfo vertex_input_stage;
	/*vertex_input_stage.vertexBindingDescriptionCount = 1;
	vertex_input_stage.pVertexBindingDescriptions = &binding_description;
	vertex_input_stage.vertexAttributeDescriptionCount = attribute_description.size();
	vertex_input_stage.pVertexAttributeDescriptions = attribute_description.data();*/

	SAFE_GET(base_pipeline, graphics_system->get_base_pipeline());

	//INPUT ASSEMBLY STAGE
	auto input_assembly_stage = base_pipeline->get_input_assembly_stage_create_info();

	//RASTERIZATION STAGE
	auto rasterization_stage = base_pipeline->get_rasterization_stage_create_info();

	//COLOR BLEND
	auto color_blend_stage = base_pipeline->get_color_blend_stage_create_info();

	//VIEWPORT STAGE
	auto viewport_stage = base_pipeline->get_viewport_stage_create_info();

	//DEPTH STENCIL STAGE
	auto depth_stencil_stage = base_pipeline->get_depth_stencil_stage_create_info();

	//MULTISAMPLE STAGE
	auto multisample_stage = base_pipeline->get_multisample_stage_create_info();

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
	graphics_pipeline_create_info.renderPass = base_pipeline->get_render_pass();
	graphics_pipeline_create_info.subpass = 0;
	graphics_pipeline_create_info.basePipelineHandle = vk::Pipeline();
	graphics_pipeline_create_info.basePipelineIndex = -1;

	GET_CONTEXT;

	m_base_pipeline = context->m_device.createGraphicsPipeline(vk::PipelineCache(), graphics_pipeline_create_info);
}
