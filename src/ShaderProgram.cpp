#include "ShaderProgram.hpp"

//------------------------------------------------------------------------

ShaderProgram::ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader, const Shader& geometryShader)
{
    m_handle = glCreateProgram();

    vertexShader.attach(m_handle);
    geometryShader.attach(m_handle);
    fragmentShader.attach(m_handle);

    link();
    
    vertexShader.detach(m_handle);
    geometryShader.detach(m_handle);
    fragmentShader.detach(m_handle);
}

//------------------------------------------------------------------------
 
ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_handle);
}

//------------------------------------------------------------------------

template<typename T>
void ShaderProgram::setUniform(const std::string& name, const void* data) noexcept
{
    std::ostringstream errMsg;
    errMsg << "Invalid uniform variable type "
           << "(" << typeid(T).name() << ")\n";
    std::cerr << errMsg.str();
}

template<>
void ShaderProgram::setUniform<int32_t>(const std::string& name, const void* data) noexcept
{
    use();
    glUniform1i(getUniformLocation(name), *reinterpret_cast<const int32_t*>(data));
    unuse();
}

template<>
void ShaderProgram::setUniform<float>(const std::string& name, const void* data) noexcept
{
    use();
    glUniform1f(getUniformLocation(name), *reinterpret_cast<const float*>(data));
    unuse();
}

template<>
void ShaderProgram::setUniform<glm::vec2>(const std::string& name, const void* data) noexcept
{
    use();
    glUniform2fv(getUniformLocation(name), 1, reinterpret_cast<const float*>(data));
    unuse();
}

template<>
void ShaderProgram::setUniform<glm::vec3>(const std::string& name, const void* data) noexcept
{
    use();
    glUniform3fv(getUniformLocation(name), 1, reinterpret_cast<const float*>(data));
    unuse();
}

template<>
void ShaderProgram::setUniform<glm::vec4>(const std::string& name, const void* data) noexcept
{
    use();
    glUniform4fv(getUniformLocation(name), 1, reinterpret_cast<const float*>(data));
    unuse();
}

template<>
void ShaderProgram::setUniform<glm::mat3>(const std::string& name, const void* data) noexcept
{
    use();
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, reinterpret_cast<const float*>(data));
    unuse();
}

template<>
void ShaderProgram::setUniform<glm::mat4x3>(const std::string& name, const void* data) noexcept
{
    use();
    glUniformMatrix4x3fv(getUniformLocation(name), 1, GL_FALSE, reinterpret_cast<const float*>(data));
    unuse();
}

template<>
void ShaderProgram::setUniform<glm::mat4>(const std::string& name, const void* data) noexcept
{
    use();
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, reinterpret_cast<const float*>(data));
    unuse();
}

//------------------------------------------------------------------------

int32_t ShaderProgram::getAttribLocation(const std::string& name) noexcept
{
    if (m_attribLocationCache.find(name) != m_attribLocationCache.end())
        return m_attribLocationCache.at(name);
    
    int32_t location = glGetAttribLocation(m_handle, name.c_str());
    if (location == -1)
    {
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

int32_t ShaderProgram::getUniformLocation(const std::string& name) noexcept
{
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
        return m_uniformLocationCache.at(name);

    int32_t location = glGetUniformLocation(m_handle, name.c_str());
    if (location == -1)
    {
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

void ShaderProgram::link() noexcept
{
    glLinkProgram(m_handle);

    glGetProgramiv(m_handle, GL_LINK_STATUS, &m_linkStatus);
    
    if (!m_linkStatus)
    {
        int32_t logLength;
        glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &logLength);
        std::string infoLog;
        infoLog.resize(static_cast<std::string::size_type>(logLength - 1));
        glGetProgramInfoLog(m_handle, logLength, nullptr, infoLog.data());
        
        std::cerr << "Error linking shader program with ID "
                  << m_handle
                  << ": "
                  << infoLog
                  << "\n";
    }
}

//------------------------------------------------------------------------
