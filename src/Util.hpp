#pragma once

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

// A slightly modified version of the "TypeSize" function from 
// "OpenGL Programming Guide: The Official Guide to Learning OpenGL, Version 4.3" by Shreiner et al., pp. 66-67
std::size_t getGlTypeSize(GLenum type)
{
    std::size_t size;

    #define CASE(enum, count, type) \
    case enum: size = count * sizeof(type); break

    switch (type)
    {
        CASE(GL_FLOAT,             1,  GLfloat);
        CASE(GL_FLOAT_VEC2,        2,  GLfloat);
        CASE(GL_FLOAT_VEC3,        3,  GLfloat);
        CASE(GL_FLOAT_VEC4,        4,  GLfloat);
        CASE(GL_INT,               1,  GLint);
        CASE(GL_INT_VEC2,          2,  GLint);
        CASE(GL_INT_VEC3,          3,  GLint);
        CASE(GL_INT_VEC4,          4,  GLint);
        CASE(GL_UNSIGNED_INT,      1,  GLuint);
        CASE(GL_UNSIGNED_INT_VEC2, 2,  GLuint);
        CASE(GL_UNSIGNED_INT_VEC3, 3,  GLuint);
        CASE(GL_UNSIGNED_INT_VEC4, 4,  GLuint);
        CASE(GL_BOOL,              1,  GLboolean);
        CASE(GL_BOOL_VEC2,         2,  GLboolean);
        CASE(GL_BOOL_VEC3,         3,  GLboolean);
        CASE(GL_BOOL_VEC4,         4,  GLboolean);
        CASE(GL_FLOAT_MAT2,        4,  GLfloat);
        CASE(GL_FLOAT_MAT2x3,      6,  GLfloat);
        CASE(GL_FLOAT_MAT2x4,      8,  GLfloat);
        CASE(GL_FLOAT_MAT3,        9,  GLfloat);
        CASE(GL_FLOAT_MAT3x2,      6,  GLfloat);
        CASE(GL_FLOAT_MAT3x4,      12, GLfloat);
        CASE(GL_FLOAT_MAT4,        16, GLfloat);
        CASE(GL_FLOAT_MAT4x2,      8,  GLfloat);
        CASE(GL_FLOAT_MAT4x3,      12, GLfloat);
        
        #undef CASE
        
        default:
            std::cerr << "Unknown type: 0x" << std::hex << type << std::endl;
            break;
    }

    return size;
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
