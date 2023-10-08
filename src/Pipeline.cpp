#include "Pipeline.hpp"

#include <iostream>
#include <fstream>
#include <format>
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

void Pipeline::checkStageProgramLinkStatus(PipelineStage::Type stage) const noexcept
{
    const GLuint program = m_stages[stage];
    if (program == 0) [[unlikely]] return;

    GLint status = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) [[unlikely]]
    {
        GLchar infoLog[LOCAL_CHAR_BUF_SIZE];
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
        std::cerr << std::format("Error preparing shader with ID {}: {}", program, infoLog);
    }
}

//------------------------------------------------------------------------

std::string Pipeline::readShaderFile(const fs::path& path) const noexcept
{
    std::ifstream shaderFile{path};
    if (shaderFile.bad()) [[unlikely]]
    {
        std::cerr << std::format("Error reading shader from '{}'\n", path.string());
        return "";
    }
    std::ostringstream osstream;
    osstream << shaderFile.rdbuf();
    return osstream.str();
}

//------------------------------------------------------------------------

void Pipeline::setupStageProgram(PipelineStage::Type stage, const fs::path& shaderPath) const noexcept
{
    const std::string shaderSource = readShaderFile(shaderPath);
    const char* shaderSourceRaw = shaderSource.c_str();

    m_stages[stage] = glCreateShaderProgramv(PipelineStage2GLShader[stage], 1, &shaderSourceRaw);
    checkStageProgramLinkStatus(stage);
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
        std::cerr << std::format("Error validating pipeline with ID {}: {}", m_name, infoLog);
    }
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
