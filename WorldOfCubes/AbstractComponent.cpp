#include "stdafx.h"
#include "AbstractComponent.h"


AbstractComponent::AbstractComponent()
{
}

AbstractComponent::~AbstractComponent()
{
}

void AbstractComponent::attached(std::shared_ptr<Entity>& entity)
{
	assert(m_uid != 0);
	m_entity = entity;
	on_attach();
}

void AbstractComponent::detached()
{
	m_entity.reset();
	on_detach();
}