#pragma once

#include <string_view>

//------------------------------------------------------------------------

namespace common
{

//------------------------------------------------------------------------

inline constexpr std::string_view SHADER_PATH = "../src/shaders/";
inline constexpr std::string_view TEXTURE_PATH = "../texture/";
inline constexpr std::string_view ASSET_PATH = "../assets/";

//------------------------------------------------------------------------

enum class ResourceManagement
{
    MANUAL = 0,
    RAII = 1
};

//------------------------------------------------------------------------

}  // namespace common

//------------------------------------------------------------------------
