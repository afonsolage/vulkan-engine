#include "stdafx.h"
#include "Scene.h"
#include "Entity.h"
#include "AbstractComponent.h"

Scene::Scene()
{

}


Scene::~Scene()
{

}

void Scene::add(std::shared_ptr<Entity> entity)
{
	for (const auto& component : entity->get_all_components())
	{
		add(component.lock());
	}
}

void Scene::add(const std::shared_ptr<AbstractComponent>& component)
{
	if (!component)
	{
		LOG_ERROR("Trying to add a null component on scene!");
		return;
	}

	auto type_index = std::type_index(typeid(*component));

	auto it = std::find_if(begin(m_component_map), end(m_component_map), [&type_index](auto const& pair)
	{
		return pair.first == type_index;
	});

	if (it == m_component_map.end())
	{
		it = add_component_list(component);
	}

	it->second.emplace_back(component);
}

ComponentMapIterator Scene::add_component_list(const std::shared_ptr<AbstractComponent>& component)
{
	auto result = m_component_map.insert(std::pair<ComponentIndex, ComponentList>(ComponentIndex(component), ComponentList()));

	if (!result.second)
	{
		LOG_FATAL("Failed to inser new component list on scene!");
		throw std::runtime_error("Failed to inser new component list on scene");
	}

	return result.first;
}