#pragma once

#include <GL/glew.h>
#include <assimp/postprocess.h>

#include <cstdint>

//------------------------------------------------------------------------

namespace constants
{

constexpr std::uint32_t STD_140_PAD_UINT          = 0;
constexpr float         STD_140_PAD_FLOAT         = 0.0f;
constexpr float         Z_FIGHT_EPSILON           = 1e-5f;
constexpr std::uint32_t CAMERA_BINDING            = 0;
constexpr std::uint32_t MODEL_BINDING             = 1;
constexpr std::uint32_t MATERIAL_BINDING          = 2;
constexpr std::uint32_t AMBIENT_LIGHT_BINDING     = 3;
constexpr std::uint32_t DIRECTIONAL_LIGHT_BINDING = 4;
constexpr std::uint32_t POINT_LIGHT_BINDING       = 5;

constexpr std::int32_t ASSIMP_LOAD_FLAGS = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;

}  // namespace constants

//------------------------------------------------------------------------
