#pragma once

#include "common.hpp"

extern "C" {
#include <GLFW/glfw3.h>
}

#include <optional>
#include <string>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

struct RenderPipelineDescriptor
{
    fs::path vertPath;
    fs::path fragPath;
    std::optional<fs::path> geomPath = std::nullopt;
};

//------------------------------------------------------------------------

class RenderPipeline
{
public:
    explicit RenderPipeline(RenderPipelineDescriptor desc);

    void bind() const noexcept { glBindProgramPipeline(m_name); }

private:
    std::string parseShaderFile(const fs::path& path) const noexcept;
    void validate() const noexcept;

    GLuint m_name;
    GLuint m_vertexStage;
    GLuint m_fragmentStage;
    GLuint m_geometryStage = 0;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
