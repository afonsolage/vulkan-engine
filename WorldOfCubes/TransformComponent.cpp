#include "stdafx.h"
#include "TransformComponent.h"
#include "Entity.h"

TransformComponent::TransformComponent()
	: m_dirty(false)
{

}


TransformComponent::~TransformComponent()
{
	auto parent = m_parent.lock();
	if (parent)
	{
		parent->remove(m_uid);
	}
}

void TransformComponent::update()
{
}

void TransformComponent::on_attach()
{
}

void TransformComponent::on_detach()
{
}

void TransformComponent::add(std::shared_ptr<TransformComponent>& child)
{
	auto this_entity = m_entity.lock();
	auto child_entity = child->get_entity().lock();

	if (!this_entity)
	{
		LOG_DEBUG("Cannot add a child on a detached parent!", m_uid);
		return;
	}

	if (!child_entity)
	{
		LOG_DEBUG("Cannot add a detached child!", child->get_uid());
		return;
	}

	if (*this_entity == *child_entity)
	{
		LOG_DEBUG("Cannot add a child on same entity! (How was you able to do that?)", child->get_uid());
		return;
	}

	auto this_parent = m_parent.lock();
	if (this_parent)
	{
		if (*this_parent == *child)
		{
			LOG_DEBUG("Cannot add parent as child component. ");
			return;
		}
	}
	this_parent.reset();

	if (exists(*child))
	{
		LOG_DEBUG("Child %d already exists!", child->get_uid());
		return;
	}

	auto child_parent = child->get_parent().lock();
	if (child_parent)
	{
		child_parent->remove(child->get_uid());
	}

	m_children.emplace_back(child);
	child->m_parent = shared_from_this();
}

void TransformComponent::remove(std::weak_ptr<TransformComponent>& child)
{
	auto sp_component = child.lock();

	if (!sp_component)
	{
		LOG_DEBUG("Attempting to remove a null child.");
		return;
	}
	else if (sp_component->m_parent.expired())
	{
		LOG_DEBUG("Attempting to remove a child without parent.");
	}

	remove(sp_component->get_uid());
	assert(sp_component->m_parent.expired());
}

void TransformComponent::remove(const uint64_t& uid)
{
	auto it = std::remove_if(begin(m_children), end(m_children), [&uid](const std::weak_ptr<TransformComponent>& ptr)
	{
		return ptr.expired() || *ptr.lock() == uid;
	});

	if (it == m_children.end())
	{
		LOG_DEBUG("Attempting to remove a non-existent child: %d.", uid);
		return;
	}
	else
	{
		for (auto tmp_it = it; tmp_it != m_children.end(); tmp_it++)
		{
			auto component = tmp_it->lock();
			if (component)
			{
				component->m_parent.reset();
			}
		}
		m_children.erase(it);
	}
}

bool TransformComponent::exists(const TransformComponent& component) const
{
	return !find_child(component).expired();
}

bool TransformComponent::exists(const uint64_t& uid) const
{
	return !find_child(uid).expired();
}

std::weak_ptr<TransformComponent> TransformComponent::find_child(const TransformComponent& component) const
{
	return find_child(component.get_uid());
}

std::weak_ptr<TransformComponent> TransformComponent::find_child(const uint64_t& uid)  const
{
	auto it = std::find_if(begin(m_children), end(m_children), [&uid](const std::weak_ptr<TransformComponent>& ptr)
	{
		return !ptr.expired() && *ptr.lock() == uid;
	});

	if (it == m_children.end())
	{
		return std::weak_ptr<TransformComponent>();
	}
	else
	{
		return *it;
	}
}