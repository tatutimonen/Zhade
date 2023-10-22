#pragma once

#include "Handle.hpp"
#include "Mesh.hpp"

#include <glm/glm.hpp>
extern "C" {
#include <GL/glew.h>
}

#include <cstdint>
#include <span>
#include <string_view>
#include <vector>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class ResourceManager;

struct ModelDescriptor
{
    std::vector<Handle<Mesh>> meshes;
    glm::mat3x4 transformation{1.0f};
};

//------------------------------------------------------------------------

class Model
{
public:
    Model() = default;
    explicit Model(ModelDescriptor desc);

    [[nodiscard]] std::span<Handle<Mesh>> meshes() const noexcept { return m_meshes; }
    [[nodiscard]] const glm::mat3x4& transformation() const noexcept { return m_transformation; }
    [[nodiscard]] bool isDirty() const noexcept { return m_dirty; }

    void appendMeshes(std::span<Handle<Mesh>> meshes) const noexcept { return m_meshes.append_range(meshes); }
    void clean() const noexcept { m_dirty = false; }

    void setTransformation(const glm::mat3x4& transformation) const noexcept
    {
        m_transformation = transformation;
        m_dirty = true;
    }

private:
    mutable std::vector<Handle<Mesh>> m_meshes;
    mutable glm::mat3x4 m_transformation{1.0f};
    mutable bool m_dirty = true;

    friend class Scene;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
