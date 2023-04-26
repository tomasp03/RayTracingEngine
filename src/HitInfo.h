#pragma once
#include "Material.h"

struct HitInfo
{
    bool didHit;
    float dst;
    glm::vec3 hitPoint;
    glm::vec3 normal;
    Material mat;
};