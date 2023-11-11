#include "Pipeline.hpp"

#include <format>
#include <fstream>
#include <print>
#include <sstream>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Pipeline::Pipeline(PipelineDescriptor desc)
    : m_managed{desc.managed}
{
    glCreateProgramPipelines(1, &m_name);

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

void Pipeline::setupStageProgram(PipelineStage::Type stage, const fs::path& shaderPath) const noexcept
{
    const auto shaderSource = readFileContents(shaderPath);
    const char* shaderSourceRaw = shaderSource.c_str();
    m_stages[stage] = glCreateShaderProgramv(PipelineStage2GLShader[stage], 1, &shaderSourceRaw);
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
