#pragma once
#include "MathUtils.h"
#include "AbstractComponent.h"
#include "Transform.h"

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

	glm::mat4 get_model();

	void add(std::shared_ptr<TransformComponent>& child);

	void remove(std::weak_ptr<TransformComponent>& child);
	void remove(const uint64_t& child);

	bool exists(const TransformComponent& component) const;
	bool exists(const uint64_t& uid) const;

	std::weak_ptr<TransformComponent> find_child(const TransformComponent& component) const;
	std::weak_ptr<TransformComponent> find_child(const uint64_t& uid) const;
	std::weak_ptr<TransformComponent> get_parent() const { return m_parent; }

	const glm::vec3& get_local_translate() { return m_local_transform.get_translation(); }
	const glm::quat& get_local_rotation() { return m_local_transform.get_rotation(); }
	const glm::vec3& get_local_scaling() { return m_local_transform.get_scaling(); }
	glm::vec3 get_local_rotation_degrees() { return glm::degrees(glm::eulerAngles(get_local_rotation())); }

	void set_local_translate(const glm::vec3& translate) { m_local_transform.set_translation(translate); m_dirty = true; }
	void set_local_rotation(const glm::quat& rotation) { m_local_transform.set_rotation(rotation); m_dirty = true; }
	void set_local_scaling(const glm::vec3& scaling) { m_local_transform.set_scaling(scaling); m_dirty = true; }
	void set_local_rotation_degrees(const glm::vec3& rotation) { set_local_rotation(glm::radians(rotation)); }

	void add_local_translate(const glm::vec3& translate) { m_local_transform.add_translation(translate); m_dirty = true; }
	void add_local_rotation(const glm::quat& rotation) { m_local_transform.add_rotation(rotation); m_dirty = true; }
	void add_local_scaling(const glm::vec3& scaling) { m_local_transform.add_scaling(scaling); m_dirty = true; }
	void add_local_rotation_degrees(const glm::vec3& rotation) { add_local_rotation(glm::radians(rotation)); }

	const glm::vec3& get_world_translate() { check_world_transform_update(); return m_world_transform.get_translation(); }
	const glm::quat& get_world_rotation() { check_world_transform_update(); return m_world_transform.get_rotation(); }
	const glm::vec3& get_world_scaling() { check_world_transform_update(); return m_world_transform.get_scaling(); }
	glm::vec3 get_world_rotation_degrees() { return glm::degrees(glm::eulerAngles(get_world_rotation())); }

private:
	void check_world_transform_update();
	void update_world_transform();

	Transform m_local_transform;
	Transform m_world_transform;

	glm::mat4 m_model;

	bool m_dirty;

	std::vector<std::weak_ptr<TransformComponent>> m_children;
	std::weak_ptr<TransformComponent> m_parent;
};