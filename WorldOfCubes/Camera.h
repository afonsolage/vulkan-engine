#pragma once

#include "MathUtils.h"
#include "TransformComponent.h"

class GraphicsSystem;
class Context;

class Camera
{
public:
	Camera(std::shared_ptr<GraphicsSystem> graphics_system);
	virtual ~Camera();

	void setup_projection(float field_of_view, float ratio, float near_plane, float far_plane);

	glm::mat4 get_view() const noexcept { return m_view; };
	glm::mat4 get_projection() const noexcept { return m_view; };

private:
	std::weak_ptr<GraphicsSystem> m_graphics_system;
	std::weak_ptr<Context> m_context;

	TransformComponent transform;

	glm::mat4 m_view;
	glm::mat4 m_projection;

	void setup_view();

	float m_fov;
	float m_ratio;
	float m_near_plane;
	float m_far_plane;
};

