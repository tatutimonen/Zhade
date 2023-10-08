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

class Model2;

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
    Handle<Texture> diffuse{};
    Model2* model = nullptr;
    uint32_t id{};
};

//------------------------------------------------------------------------

class Mesh
{
public:
    Mesh() = default;
    explicit Mesh(MeshDescriptor desc) : m_desc{desc} {};

    auto operator<=>(const Mesh& rhs) const noexcept { id() <=> rhs.id(); }

    [[nodiscard]] std::span<Vertex> vertices() const noexcept { return m_desc.vertices; }
    [[nodiscard]] std::span<GLuint> indices() const noexcept { return m_desc.indices; }
    [[nodiscard]] Handle<Texture> diffuse() const noexcept { return m_desc.diffuse; }
    [[nodiscard]] const Model2* model() const noexcept { return m_desc.model; }
    [[nodiscard]] uint32_t id() const noexcept { return m_desc.id; }

    [[nodiscard]] GLuint numVertices() const noexcept { return static_cast<GLuint>(m_desc.vertices.size()); }
    [[nodiscard]] GLuint numIndices() const noexcept { return static_cast<GLuint>(m_desc.indices.size()); }

private:
    MeshDescriptor m_desc{};
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
