#include "stdafx.h"
#include "WorldApplicationState.h"
#include "Application.h"
#include "GameEngine.h"
#include "Camera.h"
#include "GraphicsSystem.h"
#include "ColoredMaterial.h"

WorldApplicationState::WorldApplicationState(std::shared_ptr<Application> application)
	: AbstractApplicationState(application)
{
	m_engine = application->get_engine();

	auto engine = m_engine.lock();

	if (!engine)
		LOG_FATAL("Unable to get engine!");

	m_graphics_system = engine->get_graphics_sytem();

	m_main_camera = std::make_shared<Camera>(m_graphics_system.lock());
}

WorldApplicationState::~WorldApplicationState()
{
}

void WorldApplicationState::initialize()
{
	LOG_INFO("Initializing World Application State");

	AbstractApplicationState::initialize();

	m_main_camera->setup_projection(45.0f, 800.0f / 600.0f, 1.0f, 10.0f);

	SAFE_GET(graphics_system, m_graphics_system);

	auto material = graphics_system->get_material<ColoredMaterial>().lock();
	auto ppl = material->get_pipeline();
}

void WorldApplicationState::update(float delta)
{
	AbstractApplicationState::update(delta);

}

void WorldApplicationState::terminate()
{
	LOG_INFO("Terminating World Application State");

	AbstractApplicationState::terminate();
}
