#include "RenderPipeline.hpp"

#include <iostream>
#include <fstream>
#include <format>
#include <sstream>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

RenderPipeline::RenderPipeline(RenderPipelineDescriptor desc)
    : m_managed{desc.managed}
{
    glCreateProgramPipelines(1, &m_name);

    const std::string vertSource = readShaderFile(desc.vertPath);
    const std::string fragSource = readShaderFile(desc.fragPath);
    const char* vertSourceRaw = vertSource.c_str();
    const char* fragSourceRaw = fragSource.c_str();
    m_stages[PipelineStage::VERTEX] = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &vertSourceRaw);
    m_stages[PipelineStage::FRAGMENT] = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fragSourceRaw);
    checkProgramLinkStatus(PipelineStage::VERTEX);
    checkProgramLinkStatus(PipelineStage::FRAGMENT);
    glUseProgramStages(m_name, GL_VERTEX_SHADER_BIT, m_stages[PipelineStage::VERTEX]);
    glUseProgramStages(m_name, GL_FRAGMENT_SHADER_BIT, m_stages[PipelineStage::FRAGMENT]);

    if (!desc.geomPath.empty())
    {
        const std::string geomSource = readShaderFile(desc.geomPath);
        const char* geomSourceRaw = geomSource.c_str();
        m_stages[PipelineStage::GEOMETRY] = glCreateShaderProgramv(GL_GEOMETRY_SHADER, 1, &geomSourceRaw);
        checkProgramLinkStatus(PipelineStage::GEOMETRY);
        glUseProgramStages(m_name, GL_GEOMETRY_SHADER_BIT, m_stages[PipelineStage::GEOMETRY]);
    }

    validate();
}

//------------------------------------------------------------------------

RenderPipeline::~RenderPipeline()
{
    if (m_managed) [[likely]] return;
    freeResources();
}

//------------------------------------------------------------------------

void RenderPipeline::freeResources() const noexcept
{
    glDeleteProgramPipelines(1, &m_name);
    glDeleteProgram(m_stages[PipelineStage::VERTEX]);
    glDeleteProgram(m_stages[PipelineStage::FRAGMENT]);
    glDeleteProgram(m_stages[PipelineStage::GEOMETRY]);
}

//------------------------------------------------------------------------

void RenderPipeline::checkProgramLinkStatus(PipelineStage::Type stage) const noexcept
{
    const GLuint program = m_stages[stage];
    if (program == 0) [[unlikely]] return;

    GLchar infoLog[constants::LOCAL_CHAR_BUF_SIZE] = { 0 };
    GLint status = GL_FALSE;

    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) [[unlikely]]
    {
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
        std::cerr << std::format("Error preparing shader with ID {}: {}", program, infoLog);
    }
}

//------------------------------------------------------------------------

std::string RenderPipeline::readShaderFile(const fs::path& path) const noexcept
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

void RenderPipeline::validate() const noexcept
{
    GLchar infoLog[constants::LOCAL_CHAR_BUF_SIZE] = { 0 };
    GLint status = GL_FALSE;

    glValidateProgramPipeline(m_name);
    glGetProgramPipelineiv(m_name, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE) [[unlikely]]
    {
        glGetProgramPipelineInfoLog(m_name, sizeof(infoLog), nullptr, infoLog);
        std::cerr << std::format("Error validating pipeline with ID {}: {}", m_name, infoLog);
    }
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
