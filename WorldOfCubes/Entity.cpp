#include "stdafx.h"
#include "Entity.h"
#include "AbstractComponent.h"

Entity::Entity()
{
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
