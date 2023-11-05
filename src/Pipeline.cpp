#include "Pipeline.hpp"

#include <bit>
#include <format>
#include <fstream>
#include <print>
#include <sstream>
#include <string>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Pipeline::Pipeline(PipelineDescriptor desc)
    : m_namedStrings{std::move(desc.namedStrings)},
      m_managed{desc.managed}
{
    glCreateProgramPipelines(1, &m_name);

    setupShaderHeaders();

    setupStageProgram(PipelineStage::VERTEX, desc.vertPath);
    setupStageProgram(PipelineStage::FRAGMENT, desc.fragPath);

    if (!desc.geomPath.empty())
    {   
        setupStageProgram(PipelineStage::GEOMETRY, desc.geomPath);
    }

    validate();
}

//------------------------------------------------------------------------

Pipeline::~Pipeline()
{
    if (m_managed) [[likely]] return;
    freeResources();
}

//------------------------------------------------------------------------

void Pipeline::freeResources() const noexcept
{
    glDeleteProgramPipelines(1, &m_name);
    glDeleteProgram(m_stages[PipelineStage::VERTEX]);
    glDeleteProgram(m_stages[PipelineStage::FRAGMENT]);
    glDeleteProgram(m_stages[PipelineStage::GEOMETRY]);
    for (const auto& namedString : m_namedStrings)
    {
        glDeleteNamedStringARB(namedString.size(), namedString.c_str());
    }
}

//------------------------------------------------------------------------

std::string Pipeline::readFileContents(const fs::path& path) const noexcept
{
    std::ifstream file{path};
    if (file.bad()) [[unlikely]]
    {
        std::println("Error reading shader from '{}'", path.string());
        return "";
    }
    std::ostringstream osstream;
    osstream << file.rdbuf();
    return osstream.str();
}

//------------------------------------------------------------------------

GLuint Pipeline::createShaderProgram(PipelineStage::Type stage, const std::string& shaderSource) const noexcept
{
    const GLuint shader = glCreateShader(PipelineStage2GLShader[stage]);

    const char* shaderSourceRaw = shaderSource.c_str();
    glShaderSource(shader, 1, &shaderSourceRaw, nullptr);
    static const GLchar* shaderFileSystemRoot[] = { "/" };
    glCompileShaderIncludeARB(shader, 1, shaderFileSystemRoot, nullptr);

    const GLuint program = glCreateProgram();

    GLint compileStatus = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    glProgramParameteri(program, GL_PROGRAM_SEPARABLE, GL_TRUE);
    if (compileStatus == GL_FALSE) [[unlikely]]
    {
        GLchar infoLog[LOCAL_CHAR_BUF_SIZE];
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
        std::println("Error compiling shader with ID {}: {}", program, infoLog);
        return 0;
    }

    glAttachShader(program, shader);
    glLinkProgram(program);
    glDetachShader(program, shader);
    glDeleteShader(shader);
    GLint linkStatus = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE) [[unlikely]]
    {
        GLchar infoLog[LOCAL_CHAR_BUF_SIZE];
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
        std::println("Error linking shader with ID {}: {}", program, infoLog);
        return 0;
    }

    return program;
}

//------------------------------------------------------------------------

 void Pipeline::setupShaderHeaders() const noexcept
 {
    for (const auto& namedString : m_namedStrings)
    {
        const fs::path headerPath = SHADER_PATH / fs::path{namedString}.filename();
        const std::string headerContents = readFileContents(headerPath);
        glNamedStringARB(
            GL_SHADER_INCLUDE_ARB,
            namedString.size(),
            namedString.c_str(),
            headerContents.size(),
            headerContents.c_str()
        );
    }
 }

//------------------------------------------------------------------------

void Pipeline::setupStageProgram(PipelineStage::Type stage, const fs::path& shaderPath) const noexcept
{
    m_stages[stage] = createShaderProgram(stage, readFileContents(shaderPath));
    glUseProgramStages(m_name, PipelineStage2GLShaderBit[stage], m_stages[stage]);
}

//------------------------------------------------------------------------

void Pipeline::validate() const noexcept
{
    glValidateProgramPipeline(m_name);

    GLint status = GL_FALSE;
    glGetProgramPipelineiv(m_name, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE) [[unlikely]]
    {
        GLchar infoLog[LOCAL_CHAR_BUF_SIZE];
        glGetProgramPipelineInfoLog(m_name, sizeof(infoLog), nullptr, infoLog);
        std::println("Error validating pipeline with ID {}: {}", m_name, infoLog);
    }
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
