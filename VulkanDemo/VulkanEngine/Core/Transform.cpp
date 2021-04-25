#include "Transform.h"

namespace Rendering
{
    Transform::Transform(glm::vec3 const& pos, glm::quat const& rotation, glm::vec3 const& scale)
        : m_pos(pos)
        , m_rotation(rotation)
        , m_scale(scale)
    {

    }

    glm::vec3 Transform::transformPointLocalToWorld(glm::vec3 const& point) const
    {
        return (getMatrix() * glm::vec4(point, 1.0f)).xyz();
    }

    glm::vec3 Transform::transformPointWorldToLocal(glm::vec3 const& point) const
    {
        return (getInverseMatrix() * glm::vec4(point, 1.0f)).xyz();
    }

    glm::vec3 Transform::transformVectorLocalToWorld(glm::vec3 const& vector) const
    {
        return (getMatrix() * glm::vec4(vector, 0.0f)).xyz();
    }

    glm::vec3 Transform::transformVectorWorldToLocal(glm::vec3 const& vector) const
    {
        return (getInverseMatrix() * glm::vec4(vector, 0.0f)).xyz();
    }

    glm::vec3 Transform::getRightVector() const
    {
        return transformVectorLocalToWorld(glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 Transform::getRightPoint() const
    {
        return transformPointLocalToWorld(glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 Transform::getForwardVector() const
    {
        return transformVectorLocalToWorld(glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 Transform::getForwardPoint() const
    {
        return transformPointLocalToWorld(glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 Transform::getUpVector() const
    {
        return transformVectorLocalToWorld(glm::vec3(0.0f, 0.0f, 1.0f));
    }

    glm::vec3 Transform::getUpPoint() const
    {
        return transformPointLocalToWorld(glm::vec3(0.0f, 0.0f, 1.0f));
    }

    glm::mat4 Transform::getMatrix() const
    {
        return glm::translate(glm::mat4(1.0f), m_pos) * glm::toMat4(m_rotation) * glm::scale(glm::mat4(1.0f), m_scale);
    }

    glm::mat4 Transform::getInverseMatrix() const
    {
        return glm::inverse(getMatrix());
    }
}