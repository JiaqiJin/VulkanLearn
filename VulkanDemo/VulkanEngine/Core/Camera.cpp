#include "Camera.h"

namespace Rendering
{
    glm::mat4 Camera::getViewMatrix() const
    {
        return glm::lookAt(m_transform.getPos(), m_transform.getForwardPoint(), m_transform.getUpVector());
    }

    glm::mat4 Camera::getProjectionMatrix() const
    {
        auto proj = glm::perspective(glm::radians(m_fov), m_aspect, m_nearZ, m_farZ);
        proj[1][1] *= -1;
        return proj;
    }
}