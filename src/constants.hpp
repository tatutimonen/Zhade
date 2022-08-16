#pragma once

#include <GL/glew.h>
#include <assimp/postprocess.h>

#include <cstdint>

//------------------------------------------------------------------------

namespace constants
{

//------------------------------------------------------------------------

static constexpr GLuint STD_140_PAD_UINT         = 0;
static constexpr GLfloat STD_140_PAD_FLOAT       = 0.0f;
static constexpr float Z_FIGHT_EPSILON           = 1e-5f;
static constexpr GLint CAMERA_BINDING            = 0;
static constexpr GLint MODEL_BINDING             = 1;
static constexpr GLint MATERIAL_BINDING          = 2;
static constexpr GLint AMBIENT_LIGHT_BINDING     = 3;
static constexpr GLint DIRECTIONAL_LIGHT_BINDING = 4;
static constexpr GLint POINT_LIGHT_BINDING       = 5;

static constexpr int32_t ASSIMP_LOAD_FLAGS = aiProcess_Triangulate | aiProcess_GenSmoothNormals
    | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;

}  // namespace constants

//------------------------------------------------------------------------
