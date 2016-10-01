#pragma once
#include "MathUtils.h"
#include "AbstractComponent.h"

class TransformComponent
	: public AbstractComponent
	, public std::enable_shared_from_this<TransformComponent>
{
public:
	TransformComponent();
	virtual ~TransformComponent();

	void update() override;
	void on_attach() override;
	void on_detach() override;

	bool is_dirty() { return m_dirty; }
	size_t get_child_count() { return m_children.size(); }

	void translate(const glm::vec3& direction);
	void rotate(const glm::vec3& euler_angles);

	glm::mat4 get_model() { return glm::mat4(); }

	void add(std::shared_ptr<TransformComponent>& child);

	void remove(std::weak_ptr<TransformComponent>& child);
	void remove(const uint64_t& child);

	bool exists(const TransformComponent& component) const;
	bool exists(const uint64_t& uid) const;

	std::weak_ptr<TransformComponent> find_child(const TransformComponent& component) const;
	std::weak_ptr<TransformComponent> find_child(const uint64_t& uid) const;
	std::weak_ptr<TransformComponent> get_parent() const { return m_parent; }

private:
	glm::vec3 m_position;
	glm::quat m_rotation;

	bool m_dirty;

	std::vector<std::weak_ptr<TransformComponent>> m_children;
	std::weak_ptr<TransformComponent> m_parent;
};