#pragma once

#include "UniformBuffer.hpp"

#include <GL/glew.h>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>

//------------------------------------------------------------------------
// Macros.

#define CHECK_GL_ERROR(fn) fn; checkErrors(#fn, __FILE__, __LINE__);

//------------------------------------------------------------------------
// Utilities.

namespace util
{

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

// Adapted from https://www.gamedev.net/forums/topic/685081-normal-vector-artifacts-with-nvmeshmender/5326137/.
inline std::uint32_t vec4_to_INT_2_10_10_10_REV(const glm::vec4& v) noexcept
{
    const std::uint32_t xs = v.x < 0;
    const std::uint32_t ys = v.y < 0;
    const std::uint32_t zs = v.z < 0;
    const std::uint32_t ws = v.w < 0;
    return ws << 31 | (static_cast<std::uint32_t>(v.w       + (ws << 1)) &   1) << 30 |
           zs << 29 | (static_cast<std::uint32_t>(v.z * 511 + (zs << 9)) & 511) << 20 |
           ys << 19 | (static_cast<std::uint32_t>(v.y * 511 + (ys << 9)) & 511) << 10 |
           xs << 9  | (static_cast<std::uint32_t>(v.x * 511 + (xs << 9)) & 511);
}

inline std::uint32_t makeUnitVec3xPacked() noexcept
{
    return vec4_to_INT_2_10_10_10_REV(glm::vec4(makeUnitVec3x(), 0.0f));
}

inline std::uint32_t makeNegUnitVec3xPacked() noexcept
{
    return vec4_to_INT_2_10_10_10_REV(glm::vec4(-makeUnitVec3x(), 0.0f));
}

inline std::uint32_t makeUnitVec3yPacked() noexcept
{
    return vec4_to_INT_2_10_10_10_REV(glm::vec4(makeUnitVec3y(), 0.0f));
}

inline std::uint32_t makeNegUnitVec3yPacked() noexcept
{
    return vec4_to_INT_2_10_10_10_REV(glm::vec4(-makeUnitVec3y(), 0.0f));
}

inline std::uint32_t makeUnitVec3zPacked() noexcept
{
    return vec4_to_INT_2_10_10_10_REV(glm::vec4(makeUnitVec3z(), 0.0f));
}

inline std::uint32_t makeNegUnitVec3zPacked() noexcept
{
    return vec4_to_INT_2_10_10_10_REV(glm::vec4(-makeUnitVec3z(), 0.0f));
}

}  // namespace util

//------------------------------------------------------------------------

namespace
{

//------------------------------------------------------------------------

void logGlError(GLenum err, const char* fn, const char* file, int line) noexcept
{
    std::ostream& log_stream = std::cout;
    log_stream << "OpenGL Error "
               << "0x" << std::hex << std::setw(4) << std::setfill('0') << err
               << " in call \"" << fn << "\" in "
               << file << ":" << std::dec << line 
               << std::endl;
}

void checkErrors(const char* fn, const char* file, int line)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        logGlError(err, fn, file, line);
        throw std::runtime_error(nullptr);
    }
}
    
//------------------------------------------------------------------------

}  // namespace

//------------------------------------------------------------------------
