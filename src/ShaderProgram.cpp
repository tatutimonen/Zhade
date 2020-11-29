#include "ShaderProgram.hpp"

ShaderProgram::ShaderProgram(const Shader& vertex_shader,
                             const Shader& fragment_shader,
                             const Shader* geometry_shader)
{
    m_handle = glCreateProgram();

    m_shader_handles[VERTEX_SHADER] = vertex_shader.get_handle();
    vertex_shader.attach(this);

    fragment_shader.attach(this);
    m_shader_handles[FRAGMENT_SHADER] = fragment_shader.get_handle();

    if (geometry_shader != nullptr) {
        m_shader_handles[GEOMETRY_SHADER] = geometry_shader->get_handle();
        geometry_shader->attach(this);
    }

    link();
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_handle);
    delete_shaders();
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
    for (const auto shader_handle : m_shader_handles)
        glDeleteShader(shader_handle);
}