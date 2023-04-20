#pragma once

#include "constants.hpp"

#include <glm/glm.hpp>
extern "C" {
#include <GL/glew.h>
}

#include <string>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

inline const std::string SHADER_PATH = "../src/shaders/";
inline const std::string TEXTURE_PATH = "../texture/";
inline const std::string ASSET_PATH = "../assets/";

//------------------------------------------------------------------------

enum class ResourceManagement
{
    MANUAL = 0,
    RAII = 1
};

//------------------------------------------------------------------------

struct Vertex
{
    glm::vec3 pos = glm::vec3();
    glm::vec3 nrm = glm::vec3();
    glm::vec2 uv = glm::vec2();
};

//------------------------------------------------------------------------
// As per: https://www.khronos.org/opengl/wiki/Vertex_Rendering#Indirect_rendering.

struct MultiDrawElementsIndirectCommand
{
    GLuint vertexCount;
    GLuint instanceCount;
    GLuint firstIndex;
    GLuint baseVertex;
    GLuint baseInstance;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
