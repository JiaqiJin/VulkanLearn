#pragma once

#include "Transform.h"
#include <memory>

namespace Rendering
{
    class Mesh;
    class Material;

    class SceneObject
    {
    public:
        void setMesh(std::shared_ptr<Mesh> const& mesh);
        void setMaterial(std::shared_ptr<Material> const& material);

        Transform& getTransform() { return m_transform; }
        Transform const& getTransform() const { return m_transform; }

        Mesh const& getMesh() const { return *m_mesh; }
        Material const& getMaterial() const { return *m_material; }
        bool isValid() const { return m_mesh && m_material; }

    private:
        Transform m_transform;

        std::shared_ptr<Mesh> m_mesh;
        std::shared_ptr<Material> m_material;
    };
}