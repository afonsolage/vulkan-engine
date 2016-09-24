#include "stdafx.h"
#include "CameraObject.h"
#include "GraphicsSystem.h"
#include "Context.h"

CameraObject::CameraObject(std::shared_ptr<GraphicsSystem> graphics_system)
	: m_graphics_system(graphics_system)
	, m_context(graphics_system->get_context())
{

}


CameraObject::~CameraObject()
{

}

void CameraObject::setup(float field_of_view, float ratio, float near_plane, float far_plane)
{
	m_fov = glm::radians(field_of_view);
	m_ratio = ratio;
	m_near_plane = near_plane;
	m_far_plane = far_plane;

	m_projection = glm::perspective(m_fov, m_ratio, m_near_plane, m_far_plane);
}