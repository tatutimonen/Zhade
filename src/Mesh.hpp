#pragma once

#include "constants.hpp"

#include <glm/glm.hpp>
extern "C" {
#include <GL/glew.h>
}

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

struct Material
{
    glm::vec3 diffuse = glm::vec3(1.0f);
    GLfloat _1 = constants::STD_140_PAD_FLOAT;
    glm::vec3 specular = glm::vec3(1.0f);
    GLfloat _2 = constants::STD_140_PAD_FLOAT;
    glm::vec3 ambient = glm::vec3(1.0f);
    GLfloat shininess = 1.0f;
};

//------------------------------------------------------------------------

class Mesh
{
public:
    struct Geometry
    {
        std::span<Vertex> vertices;
        std::span<GLuint> indices;
    };

    Mesh() = default;
    Mesh(const Geometry& geometry) : m_geometry{geometry} {}

    [[nodiscard]] std::span<Vertex> vertices() const noexcept { return m_geometry.vertices; }
    [[nodiscard]] std::span<GLuint> indices() const noexcept { return m_geometry.indices; }
    [[nodiscard]] size_t numVertices() const noexcept { return vertices().size(); }
    [[nodiscard]] size_t numIndices() const noexcept { return indices().size(); }

private:
    Geometry m_geometry;
    GLuint m_materialID = 0;
    GLuint m_textureID = 0;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------