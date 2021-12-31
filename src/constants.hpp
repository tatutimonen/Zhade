#pragma once

#include <GL/glew.h>

#include <cstdint>

//------------------------------------------------------------------------

namespace constants
{

constexpr uint32_t STD_140_PAD_UINT          = 0;
constexpr float    STD_140_PAD_FLOAT         = 0.0f;
constexpr float    Z_FIGHT_EPSILON           = 1e-5f;
constexpr uint32_t CAMERA_BINDING            = 0;
constexpr uint32_t AMBIENT_LIGHT_BINDING     = 1;
constexpr uint32_t DIRECTIONAL_LIGHT_BINDING = 2;
constexpr uint32_t POINT_LIGHT_BINDING       = 3;

}  // namespace constants

//------------------------------------------------------------------------
