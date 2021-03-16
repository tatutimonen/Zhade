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


namespace Util {

    inline glm::vec3 make_unit_vec3x()
    {
        return glm::vec3(1.0f, 0.0f, 0.0f);
    }
    
    inline glm::vec3 make_unit_vec3y()
    {
        return glm::vec3(0.0f, 1.0f, 0.0f);
    }
    
    inline glm::vec3 make_unit_vec3z()
    {
        return glm::vec3(0.0f, 0.0f, 1.0f);
    }

    constexpr float z_fight_epsilon = 1e-5f;

} // namespace Util

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
