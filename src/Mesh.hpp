#pragma once

#include "constants.hpp"

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
    glm::vec3 pos = glm::vec3();
    glm::vec3 nrm = glm::vec3();
    glm::vec2 uv = glm::vec2();
};

//------------------------------------------------------------------------

class Mesh
{
public:
    struct Desc
    {
        std::span<Vertex> vertices;
        std::span<GLuint> indices;
    };

    Mesh() = default;
    explicit Mesh(Desc desc) : m_desc{std::move(desc)} {}

    [[nodiscard]] std::span<Vertex> vertices() const noexcept { return m_desc.vertices; }
    [[nodiscard]] std::span<GLuint> indices() const noexcept { return m_desc.indices; }
    [[nodiscard]] size_t numVertices() const noexcept { return vertices().size(); }
    [[nodiscard]] size_t numIndices() const noexcept { return indices().size(); }

private:
    Desc m_desc;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------