#include "stdafx.h"
#include "AbstractComponent.h"


AbstractComponent::AbstractComponent()
{
}

void AbstractComponent::attached(std::shared_ptr<Entity>& entity)
{
	m_entity = entity;
	on_attach();
}

void AbstractComponent::detached()
{
	m_entity.reset();
	on_detach();
}


AbstractComponent::~AbstractComponent()
{
}
