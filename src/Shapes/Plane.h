#pragma once
#include "Shape.h"

class Plane : public Shape
{
private:
	glm::vec3 m_point;
	glm::vec3 m_normal;

public:
	glm::vec3& point() { return m_point; }
	glm::vec3* pointPtr() { return &m_point; }
	void point(glm::vec3 point) { m_point = point; }

	glm::vec3& normal() { return m_normal; }
	glm::vec3* normalPtr() { return &m_normal; }
	void normal(glm::vec3 normal) { m_normal = normal; }
	HitInfo Intersection(const Ray& ray) override;
};

