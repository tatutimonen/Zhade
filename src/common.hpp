#pragma once

#include "common_defs.h"
#include "util.hpp"

#include <assimp/postprocess.h>
#include <fmt/core.h>
#include <glm/gtx/string_cast.hpp>
extern "C" {
#include <GLFW/glfw3.h>
}

#include <bitset>
#include <cstdint>
#include <filesystem>
#include <ranges>

namespace fs = std::filesystem;

// As per the presentation by Lelbach [https://youtu.be/LW_T2RGXego].
namespace stdv = std::views;
namespace stdr = std::ranges;

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

using Keys = std::bitset<GLFW_KEY_LAST + 1>;

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 nrm;
    glm::vec2 uv;
};

//------------------------------------------------------------------------

inline constexpr size_t GIB_BYTES = 1 << 30;
inline constexpr size_t MIB_BYTES = 1 << 20;
inline constexpr size_t KIB_BYTES = 1 << 10;

inline const fs::path SOURCE_PATH  = fs::path{".."} / "src";
inline const fs::path SHADER_PATH  = SOURCE_PATH / "shaders";
inline const fs::path TEXTURE_PATH = fs::path{".."} / "texture";
inline const fs::path ASSET_PATH   = fs::path{".."} / "assets";

inline constexpr int32_t ASSIMP_LOAD_FLAGS = aiProcess_Triangulate | aiProcess_GenSmoothNormals;

inline constexpr size_t OBJECT_POOL_INIT_SIZE         = 32;
inline constexpr size_t DYNAMIC_STORAGE_GROWTH_FACTOR = 2;
inline constexpr uint16_t LOCAL_CHAR_BUF_SIZE         = 2048;

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
