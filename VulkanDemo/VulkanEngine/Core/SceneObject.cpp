#include "SceneObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"

namespace Rendering
{
    void SceneObject::setMesh(std::shared_ptr<Mesh> const& mesh)
    {
        m_mesh = mesh;
    }

    void SceneObject::setMaterial(std::shared_ptr<Material> const& material)
    {
        m_material = material;
    }
}