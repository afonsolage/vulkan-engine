#include "stdafx.h"
#include "ColoredMaterial.h"
#include "GraphicsSystem.h"
#include "Context.h"
#include "ShaderSystem.h"
#include "BaseGraphicsPipeline.h"
#include "CameraComponent.h"
#include "MeshComponent.h"

ColoredMaterial::ColoredMaterial(std::shared_ptr<GraphicsSystem> graphics_system)
	: AbstractMaterial(graphics_system)
{
	SAFE_GET(shader_system, m_shader_system);

	m_vertex_shader = shader_system->get_shader_info(ShaderSystem::Shader::eColoredVert);
	m_fragment_shader = shader_system->get_shader_info(ShaderSystem::Shader::eColoredFrag);
}

ColoredMaterial::~ColoredMaterial()
{
	auto context = m_context.lock();

	if (context)
	{
		context->m_device.destroyDescriptorPool(m_descriptor_pool);
		context->m_device.destroyDescriptorSetLayout(m_descriptor_set_layout);
	}

	LOG_DEBUG("Colored Material destroyed.");
}

void ColoredMaterial::init()
{
	AbstractMaterial::init();

	//create uniform buffer;
	//create descriptor set;
}

void ColoredMaterial::pre_render(std::shared_ptr<CameraComponent> camera)
{

}

void ColoredMaterial::render(std::shared_ptr<MeshComponent> mesh)
{

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

void ColoredMaterial::create_descriptor_pool()
{
	vk::DescriptorPoolSize pool_size;
	pool_size.descriptorCount = 1;
	pool_size.type = vk::DescriptorType::eUniformBuffer;

	vk::DescriptorPoolCreateInfo create_info;
	create_info.poolSizeCount = 1;
	create_info.pPoolSizes = &pool_size;

	GET_CONTEXT;

	m_descriptor_pool = context->m_device.createDescriptorPool(create_info);
}
