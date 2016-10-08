#pragma once

#include "Entity.h"

class CameraComponent;

class Scene : public std::enable_shared_from_this<Scene>
{
public:
	Scene();
	virtual ~Scene();

	void add(std::shared_ptr<Entity> entity);
	void update();

	void set_dirty() { m_dirty = true; }

	std::weak_ptr<CameraComponent> get_camera() { return m_camera; }

	template<typename T>
	std::vector<std::shared_ptr<T>> get_component_list();
protected:
	void remove_dead_entities();

	std::weak_ptr<CameraComponent> m_camera;
	std::vector<std::shared_ptr<Entity>> m_entities;

	bool m_dirty;
};

template<typename T>
inline std::vector<std::shared_ptr<T>> Scene::get_component_list()
{
	std::vector<std::shared_ptr<T>> list;

	for (const auto& entity : m_entities)
	{
		auto component = entity->get_component<T>().lock();
		if (component)
		{
			list.push_back(std::move(component));
		}
	}

	return list;
}
