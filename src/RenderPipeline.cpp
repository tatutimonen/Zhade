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

    [[maybe_unused]] const auto& [vertPath, fragPath, geomPath, _1] = desc;

    const std::string vertSource = readShaderFile(vertPath);
    const std::string fragSource = readShaderFile(fragPath);
    const char* vertSourceRaw = vertSource.c_str();
    const char* fragSourceRaw = fragSource.c_str();

    m_vertexStage = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &vertSourceRaw);
    m_fragmentStage = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fragSourceRaw);
    glUseProgramStages(m_name, GL_VERTEX_SHADER_BIT, m_vertexStage);
    glUseProgramStages(m_name, GL_FRAGMENT_SHADER_BIT, m_fragmentStage);

    if (geomPath)
    {
        const std::string geomSource = readShaderFile(geomPath.value());
        const char* geomSourceRaw = geomSource.c_str();
        m_geometryStage = glCreateShaderProgramv(GL_GEOMETRY_SHADER, 1, &geomSourceRaw);
        glUseProgramStages(m_name, GL_GEOMETRY_SHADER_BIT, m_geometryStage);
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
    glDeleteProgram(m_vertexStage);
    glDeleteProgram(m_fragmentStage);
    glDeleteProgram(m_geometryStage);
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
    glValidateProgramPipeline(m_name);

    GLint status;
    glGetProgramPipelineiv(m_name, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint logLength;
        glGetProgramPipelineiv(m_name, GL_INFO_LOG_LENGTH, &logLength);
        std::string infoLog;
        infoLog.resize(static_cast<std::string::size_type>(logLength - 1));
        glGetProgramPipelineInfoLog(m_name, logLength, nullptr, infoLog.data());
        std::cerr << std::format("Error validating pipeline with ID {}: {}", m_name, infoLog);
    }
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
