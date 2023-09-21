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
    ResourceManager* mngr;
    std::vector<Handle<Mesh>> meshes;
    glm::mat3x4 transformation;
    uint32_t id;
};

//------------------------------------------------------------------------

class Model2
{
public:
    Model2() = default;
    Model2(ModelDescriptor desc);

    auto operator<=>(const Model2& rhs) const noexcept { m_id <=> rhs.m_id; }

    void setTransformation(const glm::mat3x4& transformation) const noexcept { m_transformation = transformation; }

    [[nodiscard]] const glm::mat3x4& getTransformation() const noexcept { return m_transformation; }
    [[nodiscard]] uint32_t getId() const noexcept { return m_id; }

    void addMesh(const Handle<Mesh>& mesh) const noexcept { return m_meshes.push_back(mesh); }

private:
    ResourceManager* m_mngr = nullptr;
    mutable std::vector<Handle<Mesh>> m_meshes;
    mutable glm::mat3x4 m_transformation;
    uint32_t m_id;

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
