#include "stdafx.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "Scene.h"

Entity::Entity(const private_ctor& ctor)
	: m_engine(ctor.engine)
{
	generate_uid(typeid(Entity));
}

Entity::~Entity()
{
	auto scene = m_scene.lock();
	if (scene)
	{
		scene->set_dirty();
	}
}

void Entity::update()
{
	for (const auto& component : m_components)
	{
		component->update();
	}
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

std::shared_ptr<Entity> Entity::create(std::shared_ptr<GameEngine>&& engine)
{
	return std::make_shared<Entity>(private_ctor(engine));
}
