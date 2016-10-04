#pragma once

#include "UniqueIdentified.h"

class Entity;

class AbstractComponent 
	: public UniqueIdentified
{
public:
	virtual ~AbstractComponent();

	virtual void update() = 0;
	bool is_attached() const noexcept { return !m_entity.expired(); }

	std::weak_ptr<Entity> get_entity() const noexcept { return m_entity; }

	virtual uint8_t get_priority() { return std::numeric_limits<uint8_t>::max(); }

protected:
	AbstractComponent();
	AbstractComponent(const AbstractComponent& other) = delete;

	void attached(std::shared_ptr<Entity>& entity);
	void detached();

	virtual void on_attach() = 0;
	virtual void on_detach() = 0;

	
	virtual std::vector<const type_info*> get_required_components();
	virtual bool check_required_components();

	std::weak_ptr<Entity> m_entity;

	friend class Entity;
};