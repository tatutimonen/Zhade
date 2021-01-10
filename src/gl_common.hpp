#pragma once

#include <GL/glew.h>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <iomanip>
#include <string>


#define GL_CALL(fn) fn; check_errors(#fn, __FILE__, __LINE__);
#define VAR_NAME(x) std::string(#x);

constexpr float epsilon = std::numeric_limits<float>::epsilon();


inline bool vec3f_close(const glm::vec3& x, const glm::vec3& y)
{
    return glm::length2(glm::abs(x - y)) < epsilon*epsilon;
}


namespace {

    void log_gl_error(GLenum err, const char* fn, const char* file, int line)
    {
        std::ostream& log_stream = std::cout;
        log_stream << "OpenGL Error "
                   << "0x" << std::hex << std::setw(4) << std::setfill('0') << err
                   << " in call \"" << fn << "\" in "
                   << file << ":" << line 
                   << std::endl;
    }

    inline void check_errors(const char* fn, const char* file, int line)
    {
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            log_gl_error(err, fn, file, line);
            throw std::runtime_error(nullptr);
        }
    }
    
} // namespace
