#include "Camera.h"

Camera::Camera(
    glm::vec3 lookfrom,
    glm::vec3 lookat,
    glm::vec3 vup,
    float vfov, // vertical field-of-view in degrees
    float aspect_ratio
)
{
    m_position = lookfrom;
    m_positionOld = lookfrom;
    m_lookat = lookat;
    m_up = vup;
    m_settings.vfov = glm::radians(vfov);
    m_aspect_ratio = aspect_ratio;

    float h = glm::tan(m_settings.vfov / 2.0f);
    float viewport_height = 2.0f;
    float viewport_width = aspect_ratio * viewport_height;
    viewport_height *= h;

    m_orientation = glm::normalize(lookfrom - lookat);
    glm::vec3 u = glm::normalize(glm::cross(vup, m_orientation));
    glm::vec3 v = glm::cross(m_orientation, u);

    m_viewPlane.Recalculate(m_position, viewport_width * u, viewport_height * v, m_orientation);
}

void Camera::Recalculate(float aspect_ratio)
{
    m_aspect_ratio = aspect_ratio;
    float h = glm::tan(m_settings.vfov / 2.0f);
    float viewport_height = 2.0 * h;
    float viewport_width = m_aspect_ratio * viewport_height;

    m_lookat = m_lookat + m_position - m_positionOld;
    m_positionOld = m_position;
    m_orientation = glm::normalize(m_position - m_lookat);

    m_angles.y = glm::clamp(m_angles.y, glm::radians(-89.0f), glm::radians(89.0f));

    glm::quat quatx = glm::normalize(glm::angleAxis(m_angles.x, glm::vec3(0, 1, 0)));
    glm::quat quaty = glm::normalize(glm::angleAxis(m_angles.y, glm::vec3(1, 0, 0)));

    m_orientation = glm::rotate(quatx * quaty, m_orientation);
    glm::vec3 u = glm::normalize(glm::cross(m_up, m_orientation));
    glm::vec3 v = glm::cross(m_orientation, u);

    m_viewPlane.Recalculate(m_position, viewport_width * u, viewport_height * v, m_orientation);
}

void Camera::CalsculateMovement(GLFWwindow* window, float diff)
{
    double mouseX, mouseY;
    int winWidth, winHeight;
    glfwGetFramebufferSize(window, &winWidth, &winHeight);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        m_isMoving = true;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            m_position += m_settings.speed * -m_orientation * diff;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            m_position += m_settings.speed * -glm::normalize(glm::cross(m_orientation, m_up)) * diff;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            m_position += m_settings.speed * m_orientation * diff;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            m_position += m_settings.speed * glm::normalize(glm::cross(m_orientation, m_up)) * diff;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            m_position += m_settings.speed * m_up * diff;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            m_position += m_settings.speed * -m_up * diff;
        }

        if (m_firstClick)
        {
            glfwSetCursorPos(window, winWidth / 2, winHeight / 2);
            m_firstClick = false;
        }

        glfwGetCursorPos(window, &mouseX, &mouseY);
        float mouseXNorm = 2.0f * (float)mouseX / (float)winWidth - 1.0f;
        float mouseYNorm = 2.0f * (float)mouseY / (float)winHeight - 1.0f;


        m_angles.x += mouseXNorm * m_settings.mouseSensitivity * diff;
        m_angles.y += mouseYNorm * m_settings.mouseSensitivity * diff;
        m_angles.y = glm::clamp(m_angles.y, -89.0f, 89.0f);

        glfwSetCursorPos(window, winWidth / 2, winHeight / 2);

    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
    {
        m_isMoving = false;
        m_firstClick = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    }
}

Ray Camera::get_ray(float s, float t) const
{
    return Ray(m_viewPlane.origin, m_viewPlane.lower_left_corner + s * m_viewPlane.horizontal + t * m_viewPlane.vertical - m_viewPlane.origin);
}
