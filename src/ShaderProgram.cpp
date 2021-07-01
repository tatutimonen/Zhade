#include "ShaderProgram.hpp"

//------------------------------------------------------------------------

ShaderProgram::ShaderProgram(std::shared_ptr<Shader> vertexShader,
                             std::shared_ptr<Shader> fragmentShader,
                             std::shared_ptr<Shader> geometryShader)
{
    GL_CALL(m_handle = glCreateProgram());

    m_shaders[Shader::VERTEX_SHADER] = vertexShader;
    attachShader(vertexShader);

    m_shaders[Shader::FRAGMENT_SHADER] = fragmentShader;
    attachShader(fragmentShader);

    if (geometryShader != nullptr)
    {
        m_shaders[Shader::GEOMETRY_SHADER] = geometryShader;
        attachShader(geometryShader);
    }

    link();
}

ShaderProgram::~ShaderProgram()
{
    GL_CALL(glDeleteProgram(m_handle));
}

//------------------------------------------------------------------------

template<typename T>
void ShaderProgram::setUniform(const std::string& name, const GLvoid* data)
{
    std::ostringstream errMsg;
    errMsg << "Invalid uniform variable type "
           << "(" << typeid(T).name() << ")\n";
    std::cerr << errMsg.str();
}

template<>
void ShaderProgram::setUniform<GLint>(const std::string& name, const GLvoid* data)
{
    use();
    GL_CALL(glUniform1i(getUniformLocation(name), *((const GLint*)data)))
    unuse();
}

template<>
void ShaderProgram::setUniform<GLfloat>(const std::string& name, const GLvoid* data)
{
    use();
    GL_CALL(glUniform1f(getUniformLocation(name), *((const GLfloat*)data)));
    unuse();
}

template<>
void ShaderProgram::setUniform<glm::vec2>(const std::string& name, const GLvoid* data)
{
    use();
    GL_CALL(glUniform2fv(getUniformLocation(name), 1, (const GLfloat*)data));
    unuse();
}

template<>
void ShaderProgram::setUniform<glm::vec3>(const std::string& name, const GLvoid* data)
{
    use();
    GL_CALL(glUniform3fv(getUniformLocation(name), 1, (const GLfloat*)data));
    unuse();
}

template<>
void ShaderProgram::setUniform<glm::vec4>(const std::string& name, const GLvoid* data)
{
    use();
    GL_CALL(glUniform4fv(getUniformLocation(name), 1, (const GLfloat*)data));
    unuse();
}

template<>
void ShaderProgram::setUniform<glm::mat3>(const std::string& name, const GLvoid* data)
{
    use();
    GL_CALL(glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, (const GLfloat*)data));
    unuse();
}

template<>
void ShaderProgram::setUniform<glm::mat4x3>(const std::string& name, const GLvoid* data)
{
    use();
    GL_CALL(glUniformMatrix4x3fv(getUniformLocation(name), 1, GL_FALSE, (const GLfloat*)data));
    unuse();
}

template<>
void ShaderProgram::setUniform<glm::mat4>(const std::string& name, const GLvoid* data)
{
    use();
    GL_CALL(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, (const GLfloat*)data));
    unuse();
}

//------------------------------------------------------------------------

GLint ShaderProgram::getAttribLocation(const std::string& name)
{
    if (m_attribLocationCache.find(name) != m_attribLocationCache.end())
        return m_attribLocationCache[name];
    
    GL_CALL(GLint location = glGetAttribLocation(m_handle, name.c_str()));
    if (location == -1) {
        std::ostringstream errMsg;
        errMsg << "Error querying attribute location of "
               << "\"" << name << "\". "
               << "Did it go unused?\n";
        std::cerr << errMsg.str();
    }
    
    m_attribLocationCache[name] = location;
    return location;
}

//------------------------------------------------------------------------

GLint ShaderProgram::getUniformLocation(const std::string& name)
{
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
        return m_uniformLocationCache[name];

    GL_CALL(GLint location = glGetUniformLocation(m_handle, name.c_str()));
    if (location == -1) {
        std::ostringstream errMsg;
        errMsg << "Error querying uniform location of "
               << "\"" << name << "\". "
               << "Did it go unused?\n";
        std::cerr << errMsg.str();
    }

    m_uniformLocationCache[name] = location;
    return location;
}

//------------------------------------------------------------------------

void ShaderProgram::attachShader(const std::shared_ptr<Shader> shader)
{
    GL_CALL(glAttachShader(m_handle, shader->getHandle()));
}

//------------------------------------------------------------------------

void ShaderProgram::detachShader(const std::shared_ptr<Shader> shader)
{
    if (shader == nullptr)
        return;

    GL_CALL(glDetachShader(m_handle, shader->getHandle()));
    m_shaders[shader->getShaderType()] = nullptr;
}

//------------------------------------------------------------------------

void ShaderProgram::detachShaders()
{
    for (const auto& shader : m_shaders)
        detachShader(shader);
}

//------------------------------------------------------------------------

void ShaderProgram::use() const noexcept
{
    GL_CALL(glUseProgram(m_handle));
}

//------------------------------------------------------------------------

void ShaderProgram::unuse() const noexcept
{
    GL_CALL(glUseProgram(0));
}

//------------------------------------------------------------------------

void ShaderProgram::link() const
{
    GL_CALL(glLinkProgram(m_handle));

    GLint status;
    GL_CALL(glGetProgramiv(m_handle, GL_LINK_STATUS, &status));
    if (!status) {
        GLint logLength;
        GL_CALL(glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &logLength));
        std::string infoLog;
        infoLog.resize(static_cast<std::string::size_type>(logLength - 1));
        GL_CALL(glGetProgramInfoLog(m_handle, logLength, nullptr, infoLog.data()));
        
        std::ostringstream errMsg;
        errMsg << "Error linking shader program with ID "
               << m_handle
               << ": "
               << infoLog
               << "\n";
        throw std::runtime_error(errMsg.str());
    }
}

//------------------------------------------------------------------------