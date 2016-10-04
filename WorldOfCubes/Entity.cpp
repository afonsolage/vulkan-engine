#include "stdafx.h"
#include "Entity.h"
#include "TransformComponent.h"

Entity::Entity(const private_ctor& ctor)
	: m_engine(ctor.engine)
{
	generate_uid(typeid(Entity));
}

Entity::~Entity()
{
}

std::vector<std::weak_ptr<AbstractComponent>> Entity::get_all_components()
{
	std::vector<std::weak_ptr<AbstractComponent>> result;

	for (const auto& component : m_components)
	{
		result.emplace_back(component);
	}

	return result;
}

bool Entity::is_component_attached(const type_info* pinfo)
{
	for (const auto& s_ptr : m_components)
	{
		if (typeid(*s_ptr) == *pinfo)
		{
			return true;
		}
	}
	return false;
}

std::shared_ptr<Entity> Entity::create(std::shared_ptr<GameEngine>& engine)
{
	return std::make_shared<Entity>(private_ctor(engine));
}
