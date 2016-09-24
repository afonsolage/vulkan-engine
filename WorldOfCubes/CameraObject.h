#pragma once

#include "VulkanAPI.h"
#include "MathUtils.h"

class GraphicsSystem;
class Context;

class CameraObject
{
public:
	CameraObject(std::shared_ptr<GraphicsSystem> graphics_system);
	virtual ~CameraObject();

	void setup(float field_of_view, float ratio, float near_plane, float far_plane);

private:
	std::weak_ptr<GraphicsSystem> m_graphics_system;
	std::weak_ptr<Context> m_context;

	glm::mat4 m_view;
	glm::mat4 m_projection;

	float m_fov;
	float m_ratio;
	float m_near_plane;
	float m_far_plane;
};

