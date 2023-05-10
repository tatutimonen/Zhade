#pragma once

#include "Handle.hpp"
#include "Mesh.hpp"
#include "constants.hpp"

#include <glm/glm.hpp>
extern "C" {
#include <GL/glew.h>
}

#include <span>
#include <string_view>
#include <vector>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class Model2
{
public:
    Model2() = default;
    Model2(std::span<Handle<Mesh>> meshes) : m_meshes{meshes.begin(), meshes.end()} {}

    void setTransformation(const glm::mat3x4& transformation) const noexcept { m_transformation = transformation; }

    [[nodiscard]] std::span<Handle<Mesh>> getMeshes() const noexcept { return m_meshes; }
    [[nodiscard]] const glm::mat3x4& getTransformation() const noexcept { return m_transformation; }

    void addMesh(const Handle<Mesh>& mesh) const noexcept { return m_meshes.push_back(mesh); }

private:
    mutable std::vector<Handle<Mesh>> m_meshes;
    mutable glm::mat3x4 m_transformation;

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
