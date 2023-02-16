#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <functional>
#include <span>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class Vertex;

//------------------------------------------------------------------------

class Model
{
public:

private:
    std::span<Vertex> m_vertices;
    std::span<GLuint> m_indices;
    glm::mat3x4 m_transformation;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
