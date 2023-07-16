#pragma once

#include "constants.hpp"

#include <glm/glm.hpp>
extern "C" {
#include <GL/glew.h>
}

#include <array>
#include <filesystem>
namespace fs = std::filesystem;

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

inline const fs::path SHADER_PATH = fs::path{".."} / "src" / "shaders";
inline const fs::path TEXTURE_PATH = fs::path{".."} / "texture";
inline const fs::path ASSET_PATH = fs::path{".."} / "assets";

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
