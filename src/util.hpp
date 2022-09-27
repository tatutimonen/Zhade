#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <iostream>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

namespace util
{

//------------------------------------------------------------------------

glm::vec3 makeUnitVec3x() noexcept;
glm::vec3 makeUnitVec3y() noexcept;
glm::vec3 makeUnitVec3z() noexcept;

GLuint vec4_to_INT_2_10_10_10_REV(const glm::vec4& v) noexcept;

GLuint makeUnitVec3xPacked() noexcept;
GLuint makeNegUnitVec3xPacked() noexcept;
GLuint makeUnitVec3yPacked() noexcept;
GLuint makeNegUnitVec3yPacked() noexcept;
GLuint makeUnitVec3zPacked() noexcept;
GLuint makeNegUnitVec3zPacked() noexcept;

//------------------------------------------------------------------------

}  // namespace util

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------

namespace
{

//------------------------------------------------------------------------

void debugCallback([[maybe_unused]] GLenum source, [[maybe_unused]] GLenum type, [[maybe_unused]] GLuint id,
                   GLenum severity, [[maybe_unused]] GLsizei length, const char* message,
                   [[maybe_unused]] const void* userParam) noexcept
{
    if (severity == GL_DEBUG_SEVERITY_MEDIUM || severity == GL_DEBUG_SEVERITY_HIGH)
        std::cerr << message << std::endl;
}

//------------------------------------------------------------------------

}  // namespace

//------------------------------------------------------------------------
