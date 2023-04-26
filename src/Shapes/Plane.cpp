#include "Plane.h"

HitInfo Plane::Intersection(const Ray& ray)
{           
    HitInfo hitInfo;
    hitInfo.didHit = false;

    float denom = glm::dot(ray.direction, m_normal);
    if (denom == 0)
        return hitInfo;
    float numerator = glm::dot(m_point - ray.origin, m_normal);

    float t = numerator / denom;

    if (t < 0)
        return hitInfo;

    hitInfo.didHit = true;
    hitInfo.normal = m_normal;
    hitInfo.hitPoint = ray.origin + t * ray.direction;
    hitInfo.dst = t;
    hitInfo.hitPoint = hitInfo.hitPoint + hitInfo.normal * 0.00001f;

    return hitInfo;
}
