#pragma once

#include "common.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <span>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class Model
{
public:
    Model() = default;
    Model(std::span<Vertex> vertices, std::span<GLuint> indices);

    [[nodiscard]] std::span<Vertex> getVertices() const noexcept { return m_vertices; }
    [[nodiscard]] std::span<GLuint> getIndices() const noexcept { return m_indices; }
    [[nodiscard]] size_t getNumVertices() const noexcept { return m_vertices.size(); }
    [[nodiscard]] size_t getNumIndices() const noexcept { return m_indices.size(); }

private:
    std::span<Vertex> m_vertices;
    std::span<GLuint> m_indices;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
