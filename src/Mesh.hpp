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

    struct Desc
    {
        Geometry geometry;
        uint32_t materialID{};
        uint32_t diffuseID{};
    };

    Mesh() = default;
    explicit Mesh(Desc desc) : m_desc{std::move(desc)} {}

    [[nodiscard]] std::span<Vertex> vertices() const noexcept { return m_desc.geometry.vertices; }
    [[nodiscard]] std::span<GLuint> indices() const noexcept { return m_desc.geometry.indices; }
    [[nodiscard]] size_t numVertices() const noexcept { return vertices().size(); }
    [[nodiscard]] size_t numIndices() const noexcept { return indices().size(); }

private:
    Desc m_desc;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------