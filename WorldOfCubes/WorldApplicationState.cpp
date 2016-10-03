#include "stdafx.h"
#include "WorldApplicationState.h"
#include "Application.h"
#include "GameEngine.h"
#include "GraphicsSystem.h"
#include "ColoredMaterial.h"
#include "Entity.h"
#include "MeshComponent.h"

WorldApplicationState::WorldApplicationState(std::shared_ptr<Application> application)
	: AbstractApplicationState(application)
{
	auto engine = m_engine.lock();

	if (!engine)
	{
		LOG_FATAL("Unable to get engine!");
	}

	m_graphics_system = engine->get_graphics_sytem();

	//m_main_camera = std::make_shared<CameraComponent>(m_graphics_system.lock());
}

WorldApplicationState::~WorldApplicationState()
{
}

void WorldApplicationState::on_initialized()
{
	LOG_INFO("Initializing World Application State");

	//m_main_camera->setup_projection(45.0f, 800.0f / 600.0f, 1.0f, 10.0f);

	SAFE_GET(graphics_system, m_graphics_system);

	auto material = graphics_system->get_material<ColoredMaterial>().lock();
	auto ppl = material->get_pipeline();

}

void WorldApplicationState::on_updated(float delta)
{
	
}

void WorldApplicationState::on_terminated()
{
	LOG_INFO("Terminating World Application State");
}
