#pragma once

#include "common.hpp"

#include <assimp/vector3.h>
#include <glm/glm.hpp>
extern "C" {
#include <GL/glew.h>
}

#include <span>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------
// As described by Logan Smith [https://youtu.be/SmlLdd1Q2V8].

template<typename T>
[[nodiscard]] constexpr T implicit_cast(std::type_identity_t<T> val) noexcept
{
    return val;
}

//------------------------------------------------------------------------

namespace util
{

//------------------------------------------------------------------------

[[nodiscard]] constexpr auto divup(std::integral auto a, std::integral auto b) noexcept
{
    return (a + b - 1) / b;
}

[[nodiscard]] constexpr auto roundup(std::integral auto a, std::integral auto b) noexcept
{
    return divup(a, b) * b;
}

//------------------------------------------------------------------------

[[nodiscard]] inline glm::vec3 makeUnitVec3x() noexcept
{
    return glm::vec3(1.0f, 0.0f, 0.0f);
}

[[nodiscard]] inline glm::vec3 makeUnitVec3y() noexcept
{
    return glm::vec3(0.0f, 1.0f, 0.0f);
}

[[nodiscard]] inline glm::vec3 makeUnitVec3z() noexcept
{
    return glm::vec3(0.0f, 0.0f, 1.0f);
}

// Adapted from: https://www.gamedev.net/forums/topic/685081-normal-vector-artifacts-with-nvmeshmender/5326137/.
[[nodiscard]] inline GLuint vec4_to_INT_2_10_10_10_REV(const glm::vec4& v) noexcept
{
    const GLuint xs = v.x < 0;
    const GLuint ys = v.y < 0;
    const GLuint zs = v.z < 0;
    const GLuint ws = v.w < 0;
    return ws << 31 | (implicit_cast<GLuint>(v.w       + (ws << 1)) &   1) << 30 |
           zs << 29 | (implicit_cast<GLuint>(v.z * 511 + (zs << 9)) & 511) << 20 |
           ys << 19 | (implicit_cast<GLuint>(v.y * 511 + (ys << 9)) & 511) << 10 |
           xs << 9  | (implicit_cast<GLuint>(v.x * 511 + (xs << 9)) & 511);
}

[[nodiscard]] inline GLuint makeUnitVec3xPacked() noexcept
{
    return vec4_to_INT_2_10_10_10_REV(glm::vec4(makeUnitVec3x(), 0.0f));
}

[[nodiscard]] inline GLuint makeNegUnitVec3xPacked() noexcept
{
    return vec4_to_INT_2_10_10_10_REV(glm::vec4(-makeUnitVec3x(), 0.0f));
}

[[nodiscard]] inline GLuint makeUnitVec3yPacked() noexcept
{
    return vec4_to_INT_2_10_10_10_REV(glm::vec4(makeUnitVec3y(), 0.0f));
}

[[nodiscard]] inline GLuint makeNegUnitVec3yPacked() noexcept
{
    return vec4_to_INT_2_10_10_10_REV(glm::vec4(-makeUnitVec3y(), 0.0f));
}

[[nodiscard]] inline GLuint makeUnitVec3zPacked() noexcept
{
    return vec4_to_INT_2_10_10_10_REV(glm::vec4(makeUnitVec3z(), 0.0f));
}

[[nodiscard]] inline GLuint makeNegUnitVec3zPacked() noexcept
{
    return vec4_to_INT_2_10_10_10_REV(glm::vec4(-makeUnitVec3z(), 0.0f));
}

//------------------------------------------------------------------------

[[nodiscard]] inline glm::vec3 vec3FromAiVector3D(const aiVector3D& vec) noexcept
{
    return glm::vec3(vec.x, vec.y, vec.z);
}

[[nodiscard]] inline glm::vec2 vec2FromAiVector3D(const aiVector3D& vec) noexcept
{
    return glm::vec2(vec.x, vec.y);
}

//------------------------------------------------------------------------

}  // namespace util

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
