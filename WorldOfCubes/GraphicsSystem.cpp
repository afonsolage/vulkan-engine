#include "stdafx.h"
#include "GraphicsSystem.h"
#include "Context.h"
#include "GameEngine.h"
#include "Swapchain.h"
#include "BaseGraphicsPipeline.h"
#include "ShaderSystem.h"
#include "ColoredMaterial.h"
#include "Scene.h"
#include "CameraComponent.h"
#include "MeshComponent.h"

GraphicsSystem::GraphicsSystem(std::shared_ptr<GameEngine>& engine)
	: m_engine(engine)
{
	m_context = std::make_shared<Context>(engine);
}

GraphicsSystem::~GraphicsSystem()
{
	LOG_DEBUG("Graphics system destroyed.");
}

void GraphicsSystem::init()
{
	m_context->init();

	m_shader_manager = std::make_shared<ShaderSystem>(shared_from_this());
	m_shader_manager->init();

	m_swapchain = std::make_shared<Swapchain>(shared_from_this());
	m_swapchain->init();

	m_base_pipeline = std::make_shared<BaseGraphicsPipeline>(shared_from_this());
	m_base_pipeline->init();

	init_materials();
}

void GraphicsSystem::init_materials()
{
	auto material = std::make_shared<ColoredMaterial>(shared_from_this());
	material->init();
	m_material_map.insert({ &typeid(ColoredMaterial), std::move(material) });
}

uint32_t GraphicsSystem::get_window_width() const
{
	auto engine = m_engine.lock();
	assert(engine);
	return engine->get_window_width();
}

uint32_t GraphicsSystem::get_window_height() const
{
	auto engine = m_engine.lock();
	assert(engine);
	return engine->get_window_height();
}

void GraphicsSystem::render_scene(std::shared_ptr<Scene>& scene)
{
	auto camera = scene->get_camera().lock();

	if (!camera)
	{
		LOG_WARN("There is no valid camera attached to scene. Skipping rendering.");
		return;
	}

	auto view_mat = camera->get_view();
	auto proj_mat = camera->get_projection();

	auto meshes = scene->get_component_list<MeshComponent>();

	for (const auto& mesh : meshes)
	{
		
	}
}
