#pragma once

#include <GL/glew.h>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <iomanip>
#include <string>

//------------------------------------------------------------------------
// Macros.

#define CHECK_GL_ERROR(fn) fn; checkErrors(#fn, __FILE__, __LINE__);

//------------------------------------------------------------------------

namespace Util
{

inline glm::vec3 makeUnitVec3x()
{
    return glm::vec3(1.0f, 0.0f, 0.0f);
}

inline glm::vec3 makeUnitVec3y()
{
    return glm::vec3(0.0f, 1.0f, 0.0f);
}

inline glm::vec3 makeUnitVec3z()
{
    return glm::vec3(0.0f, 0.0f, 1.0f);
}

constexpr float zFightEpsilon = 1e-5f;

} // namespace Util

//------------------------------------------------------------------------

namespace
{

void logGlError(GLenum err, const char* fn, const char* file, int line)
{
    std::ostream& log_stream = std::cout;
    log_stream << "OpenGL Error "
               << "0x" << std::hex << std::setw(4) << std::setfill('0') << err
               << " in call \"" << fn << "\" in "
               << file << ":" << std::dec << line 
               << std::endl;
}

inline void checkErrors(const char* fn, const char* file, int line)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        logGlError(err, fn, file, line);
        throw std::runtime_error(nullptr);
    }
}
    
} // namespace

//------------------------------------------------------------------------
