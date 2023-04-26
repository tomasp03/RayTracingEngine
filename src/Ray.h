#pragma once
#include <glm/glm.hpp>

struct Ray
{
	Ray(glm::vec3 origin_, glm::vec3 direction_);

	glm::vec3 origin;
	glm::vec3 direction;

	glm::vec3 At(float t);
};
