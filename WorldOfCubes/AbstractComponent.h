#pragma once

class Entity;

class AbstractComponent
{
public:
	virtual ~AbstractComponent();

	virtual void update() = 0;
	bool is_attached() const noexcept { return !m_entity.expired(); }

protected:
	AbstractComponent();
	AbstractComponent(const AbstractComponent& other) = delete;

	void attached(std::shared_ptr<Entity>& entity);
	void detached();


	virtual void on_attach() = 0;
	virtual void on_detach() = 0;

	std::weak_ptr<Entity> m_entity;

	friend class Entity;
};