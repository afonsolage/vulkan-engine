#pragma once
#include "MathUtils.h"

class Transform
{
public:
	Transform();
	~Transform();

	void translate(const glm::vec3& direction);
	void rotate(const glm::vec3& euler_angles);
	void rotate(const glm::quat& quat);
	void combine(const Transform& transform);

	const glm::vec3& get_translation() const noexcept { return m_translation; }
	const glm::quat& get_rotation() const noexcept { return m_rotation; }
	const glm::vec3& get_scaling() const noexcept { return m_scaling; }

	void set_translation(const glm::vec3& translation) { m_translation = translation; };
	void set_rotation(const glm::quat& rotation) { m_rotation = rotation; };
	void set_scaling(const glm::vec3& scaling) { m_scaling = scaling; };

	void add_translation(const glm::vec3& translation) { m_translation += translation; }
	void add_rotation(const glm::quat& rotation) { m_rotation *= rotation; }
	void add_scaling(const glm::vec3& scaling) { m_scaling *= scaling; }

	glm::mat4 to_model() const noexcept;
	glm::mat4 to_view() const noexcept;
private:
	glm::vec3 m_translation;
	glm::quat m_rotation;
	glm::vec3 m_scaling;
};