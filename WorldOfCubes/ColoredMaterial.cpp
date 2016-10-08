#include "stdafx.h"
#include "ColoredMaterial.h"
#include "GraphicsSystem.h"
#include "Context.h"
#include "ShaderSystem.h"
#include "BaseGraphicsPipeline.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "Entity.h"
#include "MeshComponent.h"

ColoredMaterial::ColoredMaterial(std::shared_ptr<GraphicsSystem> graphics_system)
	: AbstractMaterial(graphics_system)
	, m_mvp(graphics_system->get_context())
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
		context->m_device.destroyBuffer(m_uniform_buffer);
		context->m_device.destroyBuffer(m_staging_uniform_buffer);
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

void ColoredMaterial::pre_render(std::shared_ptr<CameraComponent>& camera)
{
	m_mvp.set_projection(camera->get_projection());
	m_mvp.set_view(camera->get_view());
}

void ColoredMaterial::render(vk::CommandBuffer& cmd_buffer, std::shared_ptr<Entity>& entity)
{
	if (!entity)
	{
		LOG_WARN("Invalid entity received!");
		return;
	}

	auto transform = entity->get_component<TransformComponent>().lock();

	if (!transform)
	{
		return;
	}

	m_mvp.set_model(transform->get_model());
	m_mvp.sync_device_memory();

	auto mesh = entity->get_component<MeshComponent>().lock();

	if (!mesh)
	{
		LOG_WARN("Received entity with no mesh component!");
		return;
	}


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

void ColoredMaterial::create_buffers()
{
	SAFE_GET(vertex_shader, m_vertex_shader);

	vk::BufferCreateInfo create_info;
	//create_info.size = vertex_shader->m_buffer.get_stride();
}
