#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <iostream>
#include <iomanip>

//------------------------------------------------------------------------

#define CHECK_GL_ERROR(fn) fn; checkErrors(#fn, __FILE__, __LINE__);

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

void logGlError(GLenum err, const char* fn, const char* file, int line) noexcept
{
    std::ostream& log_stream = std::cout;
    log_stream << "OpenGL Error "
               << "0x" << std::hex << std::setw(4) << std::setfill('0') << err
               << " in call \"" << fn << "\" in "
               << file << ":" << std::dec << line 
               << std::endl;
}

//------------------------------------------------------------------------

void checkErrors(const char* fn, const char* file, int line)
{
    const GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        logGlError(err, fn, file, line);
        throw std::runtime_error(nullptr);
    }
}

//------------------------------------------------------------------------

}  // namespace

//------------------------------------------------------------------------
