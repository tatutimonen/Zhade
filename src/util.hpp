#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

namespace util
{

//------------------------------------------------------------------------

inline glm::vec3 makeUnitVec3x() noexcept
{
    return glm::vec3(1.0f, 0.0f, 0.0f);
}

inline glm::vec3 makeUnitVec3y() noexcept
{
    return glm::vec3(0.0f, 1.0f, 0.0f);
}

inline glm::vec3 makeUnitVec3z() noexcept
{
    return glm::vec3(0.0f, 0.0f, 1.0f);
}

// Adapted from: https://www.gamedev.net/forums/topic/685081-normal-vector-artifacts-with-nvmeshmender/5326137/.
inline GLuint vec4_to_INT_2_10_10_10_REV(const glm::vec4& v) noexcept
{
    const GLuint xs = v.x < 0;
    const GLuint ys = v.y < 0;
    const GLuint zs = v.z < 0;
    const GLuint ws = v.w < 0;
    return ws << 31 | (static_cast<GLuint>(v.w       + (ws << 1)) &   1) << 30 |
           zs << 29 | (static_cast<GLuint>(v.z * 511 + (zs << 9)) & 511) << 20 |
           ys << 19 | (static_cast<GLuint>(v.y * 511 + (ys << 9)) & 511) << 10 |
           xs << 9  | (static_cast<GLuint>(v.x * 511 + (xs << 9)) & 511);
}

inline GLuint makeUnitVec3xPacked() noexcept
{
    return vec4_to_INT_2_10_10_10_REV(glm::vec4(makeUnitVec3x(), 0.0f));
}

inline GLuint makeNegUnitVec3xPacked() noexcept
{
    return vec4_to_INT_2_10_10_10_REV(glm::vec4(-makeUnitVec3x(), 0.0f));
}

inline GLuint makeUnitVec3yPacked() noexcept
{
    return vec4_to_INT_2_10_10_10_REV(glm::vec4(makeUnitVec3y(), 0.0f));
}

inline GLuint makeNegUnitVec3yPacked() noexcept
{
    return vec4_to_INT_2_10_10_10_REV(glm::vec4(-makeUnitVec3y(), 0.0f));
}

inline GLuint makeUnitVec3zPacked() noexcept
{
    return vec4_to_INT_2_10_10_10_REV(glm::vec4(makeUnitVec3z(), 0.0f));
}

inline GLuint makeNegUnitVec3zPacked() noexcept
{
    return vec4_to_INT_2_10_10_10_REV(glm::vec4(-makeUnitVec3z(), 0.0f));
}

//------------------------------------------------------------------------

}  // namespace util

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
