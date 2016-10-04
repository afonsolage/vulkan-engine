#pragma once

#include "AbstractComponent.h"

class Entity;
class AbstractComponent;

class ComponentIndex
{
public:
	std::type_index m_index;
	uint8_t m_priority;

	ComponentIndex() = delete;

	ComponentIndex(const std::shared_ptr<AbstractComponent>& component)
		: m_index(typeid(*component))
		, m_priority(component->get_priority())
	{
	}

	bool operator<(const ComponentIndex& other) const
	{
		return (m_priority == other.m_priority) ? m_index < other.m_index : m_priority < other.m_priority;
	}

	bool operator==(const ComponentIndex& other) const
	{
		return m_index == other.m_index && m_priority == other.m_priority;
	}

	bool operator==(const std::type_index& other) const
	{
		return m_index == other;
	}
};

using ComponentList = std::vector<std::weak_ptr<AbstractComponent>>;
using ComponentMap = std::map<ComponentIndex, ComponentList>;
using ComponentMapIterator = ComponentMap::iterator;

class Scene
{
public:
	Scene();
	virtual ~Scene();

	void add(std::shared_ptr<Entity> entity);

protected:
	void add(const std::shared_ptr<AbstractComponent>& component);
	ComponentMapIterator add_component_list(const std::shared_ptr<AbstractComponent>& component);


	ComponentMap m_component_map;
};