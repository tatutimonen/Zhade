#include "Shader.hpp"

Shader::Shader(GLint gl_shader_type, const std::string& filename)
{
    std::ifstream shader_file;
    shader_file.open(filename);
    
    std::stringstream sstream;
    sstream << shader_file.rdbuf();
    m_shader_source = sstream.str();

    GL_CALL(m_handle = glCreateShader(gl_shader_type));
    const char* shader_source = m_shader_source.c_str();
    GL_CALL(glShaderSource(m_handle, 1, &shader_source, nullptr));
    GL_CALL(glCompileShader(m_handle));
    GLint status;
    GL_CALL(glGetShaderiv(m_handle, GL_COMPILE_STATUS, &status));
    if (status == GL_FALSE) {
        GLchar info_log[512];
        GL_CALL(glGetShaderInfoLog(m_handle, 512, nullptr, info_log));
        std::cout << "Error compiling shader with ID "
                  << m_handle
                  << ": "
                  << info_log;
        delete this;
    }
}

Shader::~Shader()
{
    GL_CALL(glDeleteShader(m_handle));
}

GLuint Shader::get_handle() const
{
    return m_handle;
}

void Shader::attach(const ShaderProgram* shader_program)
{
    GL_CALL(glAttachShader(shader_program->get_handle(), m_handle));
}
