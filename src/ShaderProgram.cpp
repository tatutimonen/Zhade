#include "ShaderProgram.hpp"

ShaderProgram::ShaderProgram(Shader* vertex_shader, Shader* fragment_shader, Shader* geometry_shader)
{
    m_handle = glCreateProgram();

    m_shaders[VERTEX_SHADER] = vertex_shader;
    vertex_shader->attach(this);

    fragment_shader->attach(this);
    m_shaders[FRAGMENT_SHADER] = fragment_shader;

    if (geometry_shader != nullptr) {
        m_shaders[GEOMETRY_SHADER] = geometry_shader;
        geometry_shader->attach(this);
    }

    link();
    delete_shaders();
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_handle);
}

uint32_t ShaderProgram::get_handle() const
{
    return m_handle;
}

void ShaderProgram::link() const
{
    glLinkProgram(m_handle);
}

void ShaderProgram::use() const
{
    glUseProgram(m_handle);
}

void ShaderProgram::delete_shaders()
{
    for (uint32_t i = 0; i < pipeline_length; ++i) {
        Shader* shader = m_shaders[i];
        if (shader != nullptr) {
            shader->~Shader();
            m_shaders[i] = nullptr;
        }
    }
}