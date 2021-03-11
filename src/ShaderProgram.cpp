#include "ShaderProgram.hpp"


ShaderProgram::ShaderProgram(const std::shared_ptr<Shader>& vertex_shader,
                             const std::shared_ptr<Shader>& fragment_shader,
                             const std::shared_ptr<Shader>& geometry_shader)
{
    GL_CALL(m_handle = glCreateProgram());

    m_shaders[Shader::VERTEX_SHADER] = vertex_shader;
    attach_shader(vertex_shader);

    m_shaders[Shader::FRAGMENT_SHADER] = fragment_shader;
    attach_shader(fragment_shader);

    if (geometry_shader != nullptr) {
        m_shaders[Shader::GEOMETRY_SHADER] = geometry_shader;
        attach_shader(geometry_shader);
    }

    link();
}

ShaderProgram::~ShaderProgram()
{
    GL_CALL(glDeleteProgram(m_handle));
}

template<typename T>
void ShaderProgram::set_uniform(const std::string& name, const GLvoid* data)
{
    std::stringstream err_msg;
    err_msg << "Invalid uniform variable type "
            << "(" << typeid(T).name() << ")\n";
    std::cerr << err_msg.str();
}

template<>
void ShaderProgram::set_uniform<GLint>(const std::string& name, const GLvoid* data)
{
    use();
    GL_CALL(glUniform1i(get_uniform_location(name), *((const GLint*)data)))
    unuse();
}

template<>
void ShaderProgram::set_uniform<GLfloat>(const std::string& name, const GLvoid* data)
{
    use();
    GL_CALL(glUniform1f(get_uniform_location(name), *((const GLfloat*)data)));
    unuse();
}

template<>
void ShaderProgram::set_uniform<glm::vec2>(const std::string& name, const GLvoid* data)
{
    use();
    GL_CALL(glUniform2fv(get_uniform_location(name), 1, (const GLfloat*)data));
    unuse();
}

template<>
void ShaderProgram::set_uniform<glm::vec3>(const std::string& name, const GLvoid* data)
{
    use();
    GL_CALL(glUniform3fv(get_uniform_location(name), 1, (const GLfloat*)data));
    unuse();
}

template<>
void ShaderProgram::set_uniform<glm::vec4>(const std::string& name, const GLvoid* data)
{
    use();
    GL_CALL(glUniform4fv(get_uniform_location(name), 1, (const GLfloat*)data));
    unuse();
}

template<>
void ShaderProgram::set_uniform<glm::mat3>(const std::string& name, const GLvoid* data)
{
    use();
    GL_CALL(glUniformMatrix3fv(get_uniform_location(name), 1, GL_FALSE, (const GLfloat*)data));
    unuse();
}

template<>
void ShaderProgram::set_uniform<glm::mat4x3>(const std::string& name, const GLvoid* data)
{
    use();
    GL_CALL(glUniformMatrix4x3fv(get_uniform_location(name), 1, GL_FALSE, (const GLfloat*)data));
    unuse();
}

template<>
void ShaderProgram::set_uniform<glm::mat4>(const std::string& name, const GLvoid* data)
{
    use();
    GL_CALL(glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, (const GLfloat*)data));
    unuse();
}

GLint ShaderProgram::get_attrib_location(const std::string& name)
{
    if (m_attrib_location_cache.find(name) != m_attrib_location_cache.end())
        return m_attrib_location_cache[name];
    
    GL_CALL(GLint location = glGetAttribLocation(m_handle, name.c_str()));
    if (location == -1) {
        std::stringstream err_msg;
        err_msg << "Error querying attribute location of "
                << "\"" << name << "\". "
                << "Did it go unused?\n";
        std::cerr << err_msg.str();
    }
    
    m_attrib_location_cache[name] = location;
    return location;
}

GLint ShaderProgram::get_uniform_location(const std::string& name)
{
    if (m_uniform_location_cache.find(name) != m_uniform_location_cache.end())
        return m_uniform_location_cache[name];

    GL_CALL(GLint location = glGetUniformLocation(m_handle, name.c_str()));
    if (location == (GLint)-1) {
        std::stringstream err_msg;
        err_msg << "Error querying uniform location of "
                << "\"" << name << "\". "
                << "Did it go unused?\n";
        std::cerr << err_msg.str();
    }

    m_uniform_location_cache[name] = location;
    return location;
}

void ShaderProgram::attach_shader(const std::shared_ptr<Shader>& shader)
{
    GL_CALL(glAttachShader(m_handle, shader->get_handle()));
}

void ShaderProgram::detach_shader(const std::shared_ptr<Shader>& shader)
{
    if (!shader)
        return;

    GL_CALL(glDetachShader(m_handle, shader->get_handle()));
    m_shaders[shader->get_shader_type()] = nullptr;
}

void ShaderProgram::detach_shaders()
{
    for (const auto& shader : m_shaders)
        detach_shader(shader);
}

void ShaderProgram::use() const
{
    GL_CALL(glUseProgram(m_handle));
}

void ShaderProgram::unuse() const
{
    GL_CALL(glUseProgram(0));
}

void ShaderProgram::link() const
{
    GL_CALL(glLinkProgram(m_handle));
    GLint status;
    GL_CALL(glGetProgramiv(m_handle, GL_LINK_STATUS, &status));
    if (!status) {
        char info_log[512];
        GL_CALL(glGetProgramInfoLog(m_handle, 512, nullptr, info_log));
        std::stringstream err_msg;
        err_msg << "Error linking shader program with ID "
                << m_handle
                << ": "
                << info_log
                << "\n";
        std::cerr << err_msg.str();
        throw std::runtime_error(err_msg.str());
    }
}
