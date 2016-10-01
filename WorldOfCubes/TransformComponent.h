#pragma once
#include "MathUtils.h"
#include "AbstractComponent.h"

class TransformComponent : public AbstractComponent
{
public:
	TransformComponent();
	virtual ~TransformComponent();

	void update() override;
	void on_attach() override;
	void on_detach() override;

	void translate(const glm::vec3& direction);
	void rotate(const glm::vec3& euler_angles);

	glm::mat4 get_model() { return glm::mat4(); }

private:

	glm::vec3 m_position;
	glm::quat m_rotation;

	
};