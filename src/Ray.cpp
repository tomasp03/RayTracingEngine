#include "Ray.h"

Ray::Ray(glm::vec3 origin_, glm::vec3 direction_)
	:
	origin(origin_),
	direction(direction_)
{ }

glm::vec3 Ray::At(float t)
{
	return origin + t * direction;
}