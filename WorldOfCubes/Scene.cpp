#include "stdafx.h"
#include "Scene.h"
#include "CameraComponent.h"

Scene::Scene()
	: m_dirty(false)
{

}


Scene::~Scene()
{

}

void Scene::add(std::shared_ptr<Entity> entity)
{
	m_entities.emplace_back(entity);
	entity->set_scene(shared_from_this());

	auto camera = entity->get_component<CameraComponent>();

	//Check if the attached entity have a camera component.
	if (!camera.expired())
	{
		//If it does and there is no current camera on this scene.
		if (m_camera.expired())
		{
			m_camera = std::move(camera);
		}
		else
		{
			LOG_WARN("There is already a camera component on this scene. Skipping the new one.");
		}
	}
}

void Scene::update()
{
	remove_dead_entities();

	for (const auto& entity : m_entities)
	{
		if (entity)
		{
			entity->update();
		}
	}
}

void Scene::remove_dead_entities()
{
	auto it = std::find_if(begin(m_entities), end(m_entities), [](const auto& entity)
	{
		return entity;
	});

	m_entities.erase(it);
}
