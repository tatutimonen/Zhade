#include "ShaderProgram.hpp"

//------------------------------------------------------------------------

ShaderProgram::ShaderProgram(const Shader<GL_VERTEX_SHADER>& vertexShader, const Shader<GL_FRAGMENT_SHADER>& fragmentShader,
        const std::optional<Shader<GL_GEOMETRY_SHADER>>& geometryShader)
    : m_handle{glCreateProgram()}
{
    vertexShader.attach(m_handle);
    if (geometryShader) geometryShader.value().attach(m_handle);
    fragmentShader.attach(m_handle);

    link();
    
    vertexShader.detach(m_handle);
    if (geometryShader) geometryShader.value().detach(m_handle);
    fragmentShader.detach(m_handle);
}

//------------------------------------------------------------------------
 
ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_handle);
}

//------------------------------------------------------------------------

GLint ShaderProgram::getUniformLocation(const std::string& name) const noexcept
{
    if (m_uniformLocationCache.contains(name))
        return m_uniformLocationCache.at(name);

    GLint location = glGetUniformLocation(m_handle, name.c_str());
    if (location == -1)
    {
        std::ostringstream errMsg;
        std::cerr << "Error querying uniform location of "
                  << "\"" << name << "\". "
                  << "Did it go unused?\n";
    }

    m_uniformLocationCache[name] = location;
    return location;
}

//------------------------------------------------------------------------

void ShaderProgram::link() const noexcept
{
    glLinkProgram(m_handle);
    glGetProgramiv(m_handle, GL_LINK_STATUS, &m_linkStatus);
    if (!m_linkStatus)
    {
        GLint logLength;
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
