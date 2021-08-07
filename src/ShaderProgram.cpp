#include "ShaderProgram.hpp"

//------------------------------------------------------------------------

ShaderProgram::ShaderProgram(const std::shared_ptr<Shader>& vertexShader,
    const std::shared_ptr<Shader>& fragmentShader,
    const std::shared_ptr<Shader>& geometryShader)
    : Observer()
{
    CHECK_GL_ERROR(m_handle = glCreateProgram());

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

//------------------------------------------------------------------------
 
ShaderProgram::~ShaderProgram()
{
    CHECK_GL_ERROR(glDeleteProgram(m_handle));
}

//------------------------------------------------------------------------

template<typename T>
void ShaderProgram::setUniform(const std::string& name, const GLvoid* data) noexcept
{
    std::ostringstream errMsg;
    errMsg << "Invalid uniform variable type "
           << "(" << typeid(T).name() << ")\n";
    std::cerr << errMsg.str();
}

template<>
void ShaderProgram::setUniform<GLint>(const std::string& name, const GLvoid* data) noexcept
{
    use();
    CHECK_GL_ERROR(glUniform1i(getUniformLocation(name), *((const GLint*)data)))
    unuse();
}

template<>
void ShaderProgram::setUniform<GLfloat>(const std::string& name, const GLvoid* data) noexcept
{
    use();
    CHECK_GL_ERROR(glUniform1f(getUniformLocation(name), *((const GLfloat*)data)));
    unuse();
}

template<>
void ShaderProgram::setUniform<glm::vec2>(const std::string& name, const GLvoid* data) noexcept
{
    use();
    CHECK_GL_ERROR(glUniform2fv(getUniformLocation(name), 1, (const GLfloat*)data));
    unuse();
}

template<>
void ShaderProgram::setUniform<glm::vec3>(const std::string& name, const GLvoid* data) noexcept
{
    use();
    CHECK_GL_ERROR(glUniform3fv(getUniformLocation(name), 1, (const GLfloat*)data));
    unuse();
}

template<>
void ShaderProgram::setUniform<glm::vec4>(const std::string& name, const GLvoid* data) noexcept
{
    use();
    CHECK_GL_ERROR(glUniform4fv(getUniformLocation(name), 1, (const GLfloat*)data));
    unuse();
}

template<>
void ShaderProgram::setUniform<glm::mat3>(const std::string& name, const GLvoid* data) noexcept
{
    use();
    CHECK_GL_ERROR(glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, (const GLfloat*)data));
    unuse();
}

template<>
void ShaderProgram::setUniform<glm::mat4x3>(const std::string& name, const GLvoid* data) noexcept
{
    use();
    CHECK_GL_ERROR(glUniformMatrix4x3fv(getUniformLocation(name), 1, GL_FALSE, (const GLfloat*)data));
    unuse();
}

template<>
void ShaderProgram::setUniform<glm::mat4>(const std::string& name, const GLvoid* data) noexcept
{
    use();
    CHECK_GL_ERROR(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, (const GLfloat*)data));
    unuse();
}

//------------------------------------------------------------------------

GLint ShaderProgram::getAttribLocation(const std::string& name) noexcept
{
    if (m_attribLocationCache.find(name) != m_attribLocationCache.end())
        return m_attribLocationCache.at(name);
    
    CHECK_GL_ERROR(GLint location = glGetAttribLocation(m_handle, name.c_str()));
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

GLint ShaderProgram::getUniformLocation(const std::string& name) noexcept
{
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
        return m_uniformLocationCache.at(name);

    CHECK_GL_ERROR(GLint location = glGetUniformLocation(m_handle, name.c_str()));
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

void ShaderProgram::attachShader(const std::shared_ptr<Shader>& shader)
{
    CHECK_GL_ERROR(glAttachShader(m_handle, shader->getHandle()));
}

//------------------------------------------------------------------------

void ShaderProgram::detachShader(const std::shared_ptr<Shader>& shader)
{
    if (shader == nullptr)
        return;

    CHECK_GL_ERROR(glDetachShader(m_handle, shader->getHandle()));
    m_shaders[shader->getShaderType()] = nullptr;
}

//------------------------------------------------------------------------

void ShaderProgram::detachShaders()
{
    for (const auto& shader : m_shaders)
        detachShader(shader);
}

//------------------------------------------------------------------------

void ShaderProgram::link() const
{
    CHECK_GL_ERROR(glLinkProgram(m_handle));

    GLint status;
    CHECK_GL_ERROR(glGetProgramiv(m_handle, GL_LINK_STATUS, &status));
    
    if (!status) {
        GLint logLength;
        CHECK_GL_ERROR(glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &logLength));
        std::string infoLog;
        infoLog.resize(static_cast<std::string::size_type>(logLength - 1));
        CHECK_GL_ERROR(glGetProgramInfoLog(m_handle, logLength, nullptr, infoLog.data()));
        
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

void ShaderProgram::update(const Type::Observed::ViewProjection& message) noexcept
{
    setUniform<glm::mat4>("u_view", glm::value_ptr(message.first));
    setUniform<glm::mat4>("u_projection", glm::value_ptr(message.second));
}

//------------------------------------------------------------------------
