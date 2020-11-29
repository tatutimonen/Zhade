#include "Shader.hpp"

Shader::Shader(const std::string& filename, int32_t gl_shader_type)
{
    std::ifstream shader_file;
    shader_file.open(filename);
    std::stringstream sstream;
    sstream << shader_file.rdbuf();
    m_shader_source = sstream.str();
    m_handle = glCreateShader(gl_shader_type);
    const char* shader_source = m_shader_source.c_str();
    glShaderSource(m_handle, 1, &shader_source, nullptr);
    glCompileShader(m_handle);
}

Shader::~Shader()
{
    glDeleteShader(m_handle);
}

uint32_t Shader::get_handle() const
{
    return m_handle;
}

void Shader::attach(const ShaderProgram* shader_program) const
{
    glAttachShader(shader_program->get_handle(), m_handle);
}