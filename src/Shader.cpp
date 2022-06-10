#include "Shader.hpp"

//------------------------------------------------------------------------

Shader::Shader(const std::uint32_t glShaderType, const std::string& filename)
{
    try
    {
        const auto filenameWithRelativePath = common::shaderPath + filename;
        parseShaderFile(filenameWithRelativePath);
        m_handle = glCreateShader(glShaderType);
        // glShaderSource needs an lvalue.
        const auto shaderSourcePtr = m_shaderSource.c_str();
        glShaderSource(m_handle, 1, &shaderSourcePtr, nullptr);
        compile();
    }
    catch (const std::out_of_range& e)
    {
        std::ostringstream errMsg;
        errMsg << "\nInvalid OpenGL symbolic constant describing shader type "
               << "(0x"
               << std::hex << std::setw(4) << std::setfill('0') << glShaderType
               << ")";
        std::cerr << e.what() << std::endl;        
        throw;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

//------------------------------------------------------------------------

Shader::~Shader()
{
    glDeleteShader(m_handle);
}

//------------------------------------------------------------------------

void Shader::parseShaderFile(const std::string& filename)
{
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    shaderFile.open(filename);
    std::ostringstream osstream;
    osstream << shaderFile.rdbuf();
    m_shaderSource = osstream.str();
    shaderFile.close();
}

//------------------------------------------------------------------------

void Shader::compile() const
{
    glCompileShader(m_handle);

    std::int32_t status;
    glGetShaderiv(m_handle, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE)
    {
        int32_t logLength;
        glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &logLength);
        std::string infoLog;
        infoLog.resize(static_cast<std::string::size_type>(logLength - 1));
        glGetShaderInfoLog(m_handle, logLength, nullptr, infoLog.data());
        
        std::ostringstream errMsg;
        errMsg << "Error compiling shader with ID "
                << m_handle
                << ": "
                << infoLog;
        throw std::runtime_error(errMsg.str());
    }
}

//------------------------------------------------------------------------
