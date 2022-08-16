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

void logGlError(GLenum err, const char* fn, const char* file, int line) noexcept;
void checkErrors(const char* fn, const char* file, int line);

//------------------------------------------------------------------------

}  // namespace

//------------------------------------------------------------------------