#include "stdafx.h"
#include "Camera.h"
#include "GraphicsSystem.h"
#include "Context.h"
#include "Camera.h"

Camera::Camera(std::shared_ptr<GraphicsSystem> graphics_system)
	: m_graphics_system(graphics_system)
	, m_context(graphics_system->get_context())
{

}


Camera::~Camera()
{

}

void Camera::setup_projection(float field_of_view, float ratio, float near_plane, float far_plane)
{
	m_fov = glm::radians(field_of_view);
	m_ratio = ratio;
	m_near_plane = near_plane;
	m_far_plane = far_plane;

	m_projection = glm::perspective(m_fov, m_ratio, m_near_plane, m_far_plane);
}

void Camera::setup_view()
{
	m_view = glm::translate(transform.get_model(), glmc::vec3_minus_one);
}
