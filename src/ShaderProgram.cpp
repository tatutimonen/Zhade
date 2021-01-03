#include "ShaderProgram.hpp"


ShaderProgram::ShaderProgram(std::shared_ptr<Shader> vertex_shader,
                             std::shared_ptr<Shader> fragment_shader,
                             std::shared_ptr<Shader> geometry_shader)
{
    GL_CALL(m_handle = glCreateProgram());

    m_shaders[VERTEX_SHADER] = vertex_shader;
    vertex_shader->attach(this->get_handle());

    m_shaders[FRAGMENT_SHADER] = fragment_shader;
    fragment_shader->attach(this->get_handle());

    if (geometry_shader != nullptr) {
        m_shaders[GEOMETRY_SHADER] = geometry_shader;
        geometry_shader->attach(this->get_handle());
    }

    link();
}

ShaderProgram::~ShaderProgram()
{
    GL_CALL(glDeleteProgram(m_handle));
}

GLint ShaderProgram::get_attrib_location(const std::string& name)
{
    if (m_attrib_location_cache.find(name) != m_attrib_location_cache.end())
        return m_attrib_location_cache[name];
    
    GL_CALL(GLint location = glGetAttribLocation(m_handle, name.c_str()));
    if (location == -1) {
        std::string err_msg = std::string("Error querying attribute location of ")
                              + "\"" + name + "\".";
        throw std::runtime_error(err_msg);
    }
    
    m_attrib_location_cache[name] = location;
    return location;
}

GLint ShaderProgram::get_uniform_location(const std::string& name)
{
    if (m_uniform_location_cache.find(name) != m_uniform_location_cache.end())
        return m_uniform_location_cache[name];

    GL_CALL(GLint location = glGetUniformLocation(m_handle, name.c_str()));
    if (location == -1) {
        std::string err_msg = std::string("Error querying uniform location of ")
                              + "\"" + name + "\".";
        throw std::runtime_error(err_msg);
    }

    m_uniform_location_cache[name] = location;
    return location;
}

void ShaderProgram::link() const
{
    GL_CALL(glLinkProgram(m_handle));
    GLint status;
    GL_CALL(glGetProgramiv(m_handle, GL_LINK_STATUS, &status));
    if (status == GL_FALSE) {
        char info_log[512];
        GL_CALL(glGetProgramInfoLog(m_handle, 512, nullptr, info_log));
        std::cout << "Error linking shader program with ID "
                  << m_handle
                  << ": "
                  << info_log;
    }
}

void ShaderProgram::use() const
{
    GL_CALL(glUseProgram(m_handle));
}

void ShaderProgram::delete_shaders()
{
    std::fill(m_shaders, m_shaders + s_pipeline_length, nullptr);
}
