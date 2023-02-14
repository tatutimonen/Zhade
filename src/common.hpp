#pragma once

#include <string>

//------------------------------------------------------------------------

namespace common
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

}  // namespace common

//------------------------------------------------------------------------
