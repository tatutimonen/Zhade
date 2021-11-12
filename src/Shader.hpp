#pragma once

#include "Common.hpp"
#include "Util.hpp"

#include <GL/glew.h>

#include <fstream>
#include <iomanip>
#include <map>
#include <memory>
#include <sstream>
#include <string>

//------------------------------------------------------------------------

class Shader {
public:
    enum ShaderType : uint8_t {
        VERTEX_SHADER,
        GEOMETRY_SHADER,
        FRAGMENT_SHADER,
        N
    };

    Shader(GLint glShaderType, const std::string& filename = "default");
    ~Shader();

    inline GLuint getHandle() const noexcept         { return m_handle; }
    inline ShaderType getShaderType() const noexcept { return m_shaderType; }

    static const std::map<GLint, ShaderType> s_glShaderToCustomShader;
    static const std::map<GLint, std::string> s_glShaderToFileExtension;
    
private:
    void parseShaderFile(const std::string& filename);
    void compile() const;

    GLuint m_handle;
    ShaderType m_shaderType;
    std::string m_shaderSource;
};

//------------------------------------------------------------------------
