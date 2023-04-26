#pragma once
#include "Shape.h"

class Sphere : public Shape
{
private:
	glm::vec3 m_center;
	float m_radius;

public:
	glm::vec3& center() { return m_center; }
	void center(glm::vec3 center) { m_center = center; }
	glm::vec3* centerPtr() { return &m_center; }
	float& radius() { return m_radius; }
	void radius(float radius) { m_radius = radius; }
	float* radiusPtr() { return &m_radius; }
	HitInfo Intersection(const Ray& ray) override;

};

