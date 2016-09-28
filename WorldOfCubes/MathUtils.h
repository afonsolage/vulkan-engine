#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace glmc
{
	static const glm::vec3 vec3_up(0, 1, 0);
	static const glm::vec3 vec3_down(0, -1, 0);
	static const glm::vec3 vec3_right(1, 0, 0);
	static const glm::vec3 vec3_left(-1, 0, 0);
	static const glm::vec3 vec3_front(0, 0, 1);
	static const glm::vec3 vec3_back(0, 0, -1);
	static const glm::vec3 vec3_one(1, 1, 1);
	static const glm::vec3 vec3_minus_one(-1, -1, -1);
	static const glm::vec3 vec3_zero(0, 0, 0);

	static const glm::vec4 vec4_up(0, 1, 0, 0);
	static const glm::vec4 vec4_down(0, -1, 0, 0);
	static const glm::vec4 vec4_right(1, 0, 0, 0);
	static const glm::vec4 vec4_left(-1, 0, 0, 0);
	static const glm::vec4 vec4_front(0, 0, 1, 0);
	static const glm::vec4 vec4_back(0, 0, -1, 0);
	static const glm::vec4 vec4_one(1, 1, 1, 1);
	static const glm::vec4 vec4_minus_one(-1, -1, -1, -1);
	static const glm::vec4 vec4_zero(0, 0, 0, 0);
}