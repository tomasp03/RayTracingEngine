#include "Sphere.h"

HitInfo Sphere::Intersection(const Ray& ray) 
{
    HitInfo hitInfo;
    hitInfo.didHit = false;

    glm::vec3 offsetRayOrigin = ray.origin - m_center;

    float a = glm::dot(ray.direction, ray.direction);
    float half_b = glm::dot(offsetRayOrigin, ray.direction);
    float c = glm::dot(offsetRayOrigin, offsetRayOrigin) - m_radius * m_radius;

    float discriminant = half_b * half_b - a * c;
    if (discriminant >= 0)
    {
        float dst = (-half_b - glm::sqrt(discriminant)) / a;

        if (dst >= 0)
        {
            hitInfo.didHit = true;
            hitInfo.dst = dst;
            hitInfo.hitPoint = ray.origin + ray.direction * dst;
            hitInfo.normal = glm::normalize(hitInfo.hitPoint - m_center);
            hitInfo.hitPoint = hitInfo.hitPoint + hitInfo.normal * 0.00001f;
        }
    }

    return hitInfo;
}
