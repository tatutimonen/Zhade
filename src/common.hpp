#pragma once

#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
extern "C" {
#include <GL/glew.h>
}

#include <cstdint>
#include <filesystem>
#include <iostream>
#include <ranges>

namespace fs = std::filesystem;

// As per the presentation by Bryce Adelstein Lelbach (https://youtu.be/LW_T2RGXego).
namespace stdv = std::views;
namespace stdr = std::ranges;

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

inline constexpr size_t GIB_BYTES = 1ull << 30;
inline constexpr size_t MIB_BYTES = 1ull << 20;
inline constexpr size_t KIB_BYTES = 1ull << 10;

inline const fs::path SHADER_PATH = fs::path{".."} / "src" / "shaders";
inline const fs::path TEXTURE_PATH = fs::path{".."} / "texture";
inline const fs::path ASSET_PATH = fs::path{".."} / "assets";

inline constexpr GLuint STD_140_PAD_UINT   = 0;
inline constexpr GLfloat STD_140_PAD_FLOAT = 0.0f;
inline constexpr float Z_FIGHT_EPSILON     = 1e-5f;

inline constexpr GLint CAMERA_BINDING  = 0;
inline constexpr GLint MODEL_BINDING   = 1;
inline constexpr GLint TEXTURE_BINDING = 2;

inline constexpr int32_t ASSIMP_LOAD_FLAGS = aiProcess_Triangulate | aiProcess_GenSmoothNormals
    | aiProcess_FlipUVs;

inline constexpr size_t OBJECT_POOL_INIT_SIZE         = 32ull;
inline constexpr size_t DYNAMIC_STORAGE_GROWTH_FACTOR = 2ull;
inline constexpr uint8_t TEMPORAL_CACHE_SIZE          = 128;
inline constexpr uint16_t LOCAL_CHAR_BUF_SIZE         = 2048;

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
