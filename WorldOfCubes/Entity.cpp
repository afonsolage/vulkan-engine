#include "stdafx.h"
#include "Entity.h"
#include "TransformComponent.h"

Entity::Entity()
{
	generate_uid(typeid(Entity));
}


Entity::~Entity()
{
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
