#include "stdafx.h"
#include "AbstractComponent.h"
#include "Entity.h"


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
	
	if (!check_required_components())
	{
		throw std::invalid_argument("Required components are not present!");
	}

	on_attach();
}

void AbstractComponent::detached()
{
	m_entity.reset();
	on_detach();
}

std::vector<const type_info*> AbstractComponent::get_required_components()
{
	return std::vector<const type_info*>();
}

bool AbstractComponent::check_required_components()
{
	auto required_components = get_required_components();
	auto entity = m_entity.lock();

	if (!entity)
	{
		LOG_FATAL("Unable to validate required components. Entity is null.");
		throw std::runtime_error("Received null entity.");
	}

	auto this_info = &typeid(*this);

	for (const auto& t_info : required_components)
	{
		if (!entity->is_component_attached(t_info))
		{
			LOG_FATAL("%s component requires component %s which isn't attached.", this_info->m_name(), t_info->m_name());
			return false;
		}
	}
	return true;
}
