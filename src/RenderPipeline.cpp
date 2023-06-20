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
{
    glCreateProgramPipelines(1, &m_name);

    const auto& [vertPath, fragPath, geomPath] = desc;

    const std::string vertSource = parseShaderFile(vertPath);
    const std::string fragSource = parseShaderFile(fragPath);
    const char* vertSourceRaw = vertSource.c_str();
    const char* fragSourceRaw = fragSource.c_str();

    m_vertexStage = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &vertSourceRaw);
    m_fragmentStage = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fragSourceRaw);
    
    glUseProgramStages(m_name, GL_VERTEX_SHADER_BIT, m_vertexStage);
    glUseProgramStages(m_name, GL_FRAGMENT_SHADER_BIT, m_fragmentStage);

    if (geomPath)
    {
        const std::string geomSource = parseShaderFile(geomPath.value());
        const char* geomSourceRaw = geomSource.c_str();
        m_geometryStage = glCreateShaderProgramv(GL_GEOMETRY_SHADER, 1, &geomSourceRaw);
        glUseProgramStages(m_name, GL_GEOMETRY_SHADER_BIT, m_geometryStage);
    }

    validate();
}

//------------------------------------------------------------------------

std::string RenderPipeline::parseShaderFile(const fs::path& path) const noexcept
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
