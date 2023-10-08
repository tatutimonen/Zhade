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
    ResourceManager* mngr = nullptr;
    std::vector<Handle<Mesh>> meshes;
    glm::mat3x4 transformation{1.0f};
};

//------------------------------------------------------------------------

class Model2
{
public:
    Model2() = default;
    explicit Model2(ModelDescriptor desc) : m_desc{desc} {};

    [[nodiscard]] std::span<Handle<Mesh>> meshes() const noexcept { return m_desc.meshes; }
    [[nodiscard]] const glm::mat3x4& transformation() const noexcept { return m_desc.transformation; }
    [[nodiscard]] bool isDirty() const noexcept { return m_dirty; }

    void appendMeshes(std::span<Handle<Mesh>> meshes) const noexcept { return m_desc.meshes.append_range(meshes); }
    void clean() const noexcept { m_dirty = false; }

    void setTransformation(const glm::mat3x4& transformation) const noexcept
    {
        m_desc.transformation = transformation;
        m_dirty = true;
    }

private:
    mutable ModelDescriptor m_desc{};
    mutable bool m_dirty = true;

    friend class Scene;
};

//------------------------------------------------------------------------

class Model
{
public:
    Model() = default;
    Model(std::span<Vertex> vertices, std::span<GLuint> indices)
        : m_vertices{vertices},
          m_indices{indices}
    {}

    [[nodiscard]] std::span<Vertex> vertices() const noexcept { return m_vertices; }
    [[nodiscard]] std::span<GLuint> indices() const noexcept { return m_indices; }
    [[nodiscard]] size_t numVertices() const noexcept { return m_vertices.size(); }
    [[nodiscard]] size_t numIndices() const noexcept { return m_indices.size(); }

private:
    std::span<Vertex> m_vertices;
    std::span<GLuint> m_indices;
    glm::mat3x4 m_transformation;
    GLuint m_materialID = 0;
    GLuint m_textureID = 0;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
