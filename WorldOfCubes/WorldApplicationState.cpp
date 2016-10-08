#include "stdafx.h"
#include "WorldApplicationState.h"
#include "Application.h"
#include "GameEngine.h"
#include "GraphicsSystem.h"
#include "ColoredMaterial.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "Scene.h"
#include "CameraComponent.h"

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

	setup_camera();
	create_basic_triangle();
}

void WorldApplicationState::on_updated(float delta)
{

}

void WorldApplicationState::on_terminated()
{
	LOG_INFO("Terminating World Application State");
}

void WorldApplicationState::setup_camera()
{
	SAFE_GET(engine, m_engine);

	auto entity = engine->create_entity();
	auto transform = entity->attach<TransformComponent>(true);
	auto camera = entity->attach<CameraComponent>();

	transform->add_local_translate({ 0.0f, 0.0f, 2.0f });
	transform->set_local_rotation(glm::quat_cast(glm::lookAt(glmc::vec3_back, glmc::vec3_zero, glmc::vec3_up)));

	camera->setup(800.0f, 600.0f, 45.0f);

	m_main_camera = camera;
	m_main_scene->add(entity);
}

void WorldApplicationState::create_basic_triangle()
{
	SAFE_GET(graphics_system, m_graphics_system);
	SAFE_GET(engine, m_engine);

	auto material = graphics_system->get_material<ColoredMaterial>();

	auto entity = engine->create_entity();

	auto transform = entity->attach<TransformComponent>();
	auto mesh = entity->attach<MeshComponent>();

	mesh->set_material(material);

	std::vector<float> position =
	{
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.0f, 0.5f, 0.5f,
	};

	mesh->set_buffer(ColoredMaterial::POSITION_LOCATION, position);

	std::vector<float> color =
	{
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.0f, 0.5f, 0.5f,
	};

	mesh->set_buffer(ColoredMaterial::COLOR_LOCATION, color);
	mesh->set_indexes({ 1, 2, 3 });

	m_main_scene->add(entity);
}
