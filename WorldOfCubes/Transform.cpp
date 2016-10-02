#include "stdafx.h"
#include "Transform.h"


Transform::Transform()
	: m_translation(glmc::vec3_zero)
	, m_rotation(glmc::vec3_zero)
	, m_scaling(glmc::vec3_one)
{
}


Transform::~Transform()
{
}

void Transform::translate(const glm::vec3& direction)
{
	m_translation += direction;
}

void Transform::rotate(const glm::vec3& euler_angles)
{
	m_rotation *= glm::quat(euler_angles);
}

void Transform::rotate(const glm::quat& quat)
{
	m_rotation *= quat;
}

void Transform::combine(const Transform& transform)
{
	//Scale
	m_scaling *= transform.get_scaling();

	//Rotate
	m_rotation *= transform.get_rotation();

	//Translate - Scale, Rotate and then translate
	m_translation = transform.get_translation() + transform.get_rotation() * transform.get_scaling() * m_translation;
}

glm::mat4 Transform::to_mat4() const noexcept
{
	//Scale, rotate and than translate
	return glm::translate(glm::scale(glm::mat4x4(), m_scaling) * glm::mat4_cast(m_rotation), m_translation);
}
