#pragma once
#include "glm/glm.hpp"
#include<glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Ray.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

static struct ViewPlane
{
    glm::vec3 origin;
    glm::vec3 lower_left_corner;
    glm::vec3 horizontal;
    glm::vec3 vertical;

    void Recalculate(const glm::vec3& origin, 
                     const glm::vec3& horizontal, 
                     const glm::vec3& vertical, 
                     const glm::vec3& orientation)
    {
        this->origin = origin;
        this->horizontal = horizontal;
        this->vertical = vertical;
        this->lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - orientation;
    }
};

static struct Settings
{
    float vfov; // vertical field-of-view in RADIANS
    float speed = 10.0f;
    float mouseSensitivity = 50.0f;
};

class Camera
{
public:
    Camera(
        glm::vec3 lookfrom,
        glm::vec3 lookat,
        glm::vec3 vup,
        float vfov, // vertical field-of-view in degrees
        float aspect_ratio
    );

    Ray get_ray(float s, float t) const;

    void Recalculate(float aspect_ratio);
    bool& IsMoving() { return m_isMoving; }
    void CalsculateMovement(GLFWwindow* window, float diff);

    Settings& GetSettings() { return m_settings; }

private:
    bool m_isMoving = false;
    bool m_firstClick = true;

    ViewPlane m_viewPlane;
    Settings m_settings;

    float m_aspect_ratio;
    glm::vec3 m_position;
    glm::vec3 m_positionOld;
    glm::vec3 m_lookat;
    glm::vec3 m_up;
    glm::vec3 m_orientation;
    glm::vec2 m_angles = glm::vec2(0.0f);
};