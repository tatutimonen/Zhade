#pragma once



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

struct Vertex
{
    glm::vec3 pos{};
    glm::vec3 nrm{};
    glm::vec2 uv{};
};

struct MeshDescriptor
{
    std::span<Vertex> vertices;
    std::span<GLuint> indices;
};

//------------------------------------------------------------------------

class Mesh
{
public:
    Mesh() = default;
    explicit Mesh(MeshDescriptor desc) : m_vertices{desc.vertices}, m_indices{desc.indices} {}

    [[nodiscard]] std::span<Vertex> vertices() const noexcept { return m_vertices; }
    [[nodiscard]] std::span<GLuint> indices() const noexcept { return m_indices; }
    [[nodiscard]] size_t numVertices() const noexcept { return m_vertices.size(); }
    [[nodiscard]] size_t numIndices() const noexcept { return m_indices.size(); }

private:
    std::span<Vertex> m_vertices{};
    std::span<GLuint> m_indices{};
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------