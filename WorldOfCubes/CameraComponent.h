#pragma once

#include "MathUtils.h"
#include "AbstractComponent.h"

class TransformComponent;

class CameraComponent :
	public AbstractComponent
{
public:
	CameraComponent();
	virtual ~CameraComponent();

	void update() override;
	void setup(float width, float height, float field_of_view, float z_near = 0.1f, float z_far = 1000.0f);

	glm::mat4 get_projection() const noexcept { return m_projection; }
	glm::mat4 get_view();

protected:
	void on_attach() override;
	void on_detach() override;
	virtual std::vector<const type_info*> get_required_components() override;

	glm::mat4 m_projection;

	std::weak_ptr<TransformComponent> m_transform;
};