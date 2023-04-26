#pragma once
#include <glm/glm.hpp>

struct Material
{
    glm::vec4 albido;
    glm::vec4 specularColor;
    glm::vec4 emissionColor;
    float emissionStrength;
    float smoothness;
    float specularProbability;
    bool isMetalic = false;
    bool isLightSource = false;
};