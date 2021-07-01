#pragma once

#include "Util.hpp"

#include <GL/glew.h>

#include <fstream>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <vector>

//------------------------------------------------------------------------

class Shader {
public:
    enum ShaderType : unsigned char {
        VERTEX_SHADER = 0u,
        GEOMETRY_SHADER,
        FRAGMENT_SHADER,
        N
    };

    Shader(GLint glShaderType, const std::string& filename);
    ~Shader();

    inline GLuint getHandle() const noexcept         { return m_handle; }
    inline ShaderType getShaderType() const noexcept { return m_shaderType; }

    static const std::map<GLint, ShaderType> s_glShaderToCustomShader;
    
private:
    void parseShaderFile(const std::string& filename);
    void compile() const;

    GLuint m_handle;
    ShaderType m_shaderType;
    std::string m_shaderSource;
};

//------------------------------------------------------------------------
