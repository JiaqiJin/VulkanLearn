#pragma once

#include "Transform.h"

namespace Rendering
{
    class Light
    {
    public:
        Transform& getTransform() { return m_transform; }
        const Transform& getTransform() const { return m_transform; }

    private:
        Transform m_transform;
    };
}