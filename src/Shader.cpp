#include "Shader.hpp"

//------------------------------------------------------------------------

const std::map<GLint, Shader::ShaderType> Shader::s_glShaderToCustomShader = {
    { GL_VERTEX_SHADER, VERTEX_SHADER },
    { GL_GEOMETRY_SHADER, GEOMETRY_SHADER },
    { GL_FRAGMENT_SHADER, FRAGMENT_SHADER }
};

//------------------------------------------------------------------------

const std::map<GLint, std::string> Shader::s_glShaderToFileExtension = {
    { GL_VERTEX_SHADER, ".vert" },
    { GL_FRAGMENT_SHADER, ".frag" }
};

//------------------------------------------------------------------------

Shader::Shader(GLint glShaderType, const std::string& filename)
{
    try
    {
        m_shaderType = s_glShaderToCustomShader.at(glShaderType);
        const std::string filenameWithRelativePath =
            Common::shaderPath + (filename != "default" ? filename : filename + s_glShaderToFileExtension.at(glShaderType));
        parseShaderFile(filenameWithRelativePath);
        m_handle = glCreateShader(glShaderType);
        // glShaderSource needs an lvalue.
        const GLchar* shaderSourcePtr = m_shaderSource.c_str();
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

    GLint status;
    glGetShaderiv(m_handle, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint logLength;
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
