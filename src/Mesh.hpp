#pragma once

#include "Handle.hpp"
#include "Texture.hpp"

#include <glm/glm.hpp>
extern "C" {
#include <GL/glew.h>
}

#include <cstdint>
#include <span>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class Model;

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 nrm;
    glm::vec2 uv;
};

struct MeshDescriptor
{
    std::span<Vertex> vertices;
    std::span<GLuint> indices;
    GLuint baseVertex;
    GLuint firstIndex;
    Handle<Texture> diffuse;
    Handle<Model> model;
    uint32_t id;
};

//------------------------------------------------------------------------

class Mesh
{
public:
    Mesh() = default;
    explicit Mesh(MeshDescriptor desc);

    auto operator<=>(const Mesh& rhs) const noexcept { m_id <=> rhs.id(); }

    [[nodiscard]] std::span<Vertex> vertices() const noexcept { return m_vertices; }
    [[nodiscard]] std::span<GLuint> indices() const noexcept { return m_indices; }
    [[nodiscard]] GLuint baseVertex() const noexcept { return m_baseVertex; }
    [[nodiscard]] GLuint firstIndex() const noexcept { return m_firstIndex; }
    [[nodiscard]] Handle<Texture> diffuse() const noexcept { return m_diffuse; }
    [[nodiscard]] const Handle<Model>& model() const noexcept { return m_model; }
    [[nodiscard]] uint32_t id() const noexcept { return m_id; }

    [[nodiscard]] GLuint numVertices() const noexcept { return static_cast<GLuint>(m_vertices.size()); }
    [[nodiscard]] GLuint numIndices() const noexcept { return static_cast<GLuint>(m_indices.size()); }

private:
    std::span<Vertex> m_vertices;
    std::span<GLuint> m_indices;
    GLuint m_baseVertex;
    GLuint m_firstIndex;
    Handle<Texture> m_diffuse{};
    Handle<Model> m_model{};
    uint32_t m_id = 0;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
