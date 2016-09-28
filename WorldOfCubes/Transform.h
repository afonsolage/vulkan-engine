#pragma once
#include "MathUtils.h"

class Transform
{
public:
	Transform();
	virtual ~Transform();

	void translate(const glm::vec3& direction);
	void rotate(const glm::vec3& euler_angles);

	glm::mat4 get_model() { return glm::mat4(); }

private:
};

