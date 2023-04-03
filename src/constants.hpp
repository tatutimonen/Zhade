#pragma once

#include <assimp/postprocess.h>
extern "C" {
#include <GL/glew.h>
}

#include <cstdint>

//------------------------------------------------------------------------

namespace constants
{

//------------------------------------------------------------------------

inline constexpr GLuint STD_140_PAD_UINT         = 0;
inline constexpr GLfloat STD_140_PAD_FLOAT       = 0.0f;
inline constexpr float Z_FIGHT_EPSILON           = 1e-5f;
inline constexpr GLint CAMERA_BINDING            = 0;
inline constexpr GLint MODEL_BINDING             = 1;
inline constexpr GLint MATERIAL_BINDING          = 2;
inline constexpr GLint AMBIENT_LIGHT_BINDING     = 3;
inline constexpr GLint DIRECTIONAL_LIGHT_BINDING = 4;
inline constexpr GLint POINT_LIGHT_BINDING       = 5;

inline constexpr int32_t ASSIMP_LOAD_FLAGS = aiProcess_Triangulate | aiProcess_GenSmoothNormals
    | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;

//------------------------------------------------------------------------

}  // namespace constants

//------------------------------------------------------------------------
