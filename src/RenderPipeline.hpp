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
    bool managed = true;
};

//------------------------------------------------------------------------

class RenderPipeline
{
public:
    RenderPipeline() = default;
    explicit RenderPipeline(RenderPipelineDescriptor desc);
    ~RenderPipeline();

    RenderPipeline(const RenderPipeline&) = delete;
    RenderPipeline& operator=(const RenderPipeline&) = delete;
    RenderPipeline(RenderPipeline&& other) = delete;
    RenderPipeline& operator=(RenderPipeline&&) = delete;

    void bind() const noexcept { glBindProgramPipeline(m_name); }

private:
    void freeResources() const noexcept;
    [[nodiscard]] std::string readShaderFile(const fs::path& path) const noexcept;
    void validate() const noexcept;

    GLuint m_name = 0;
    GLuint m_vertexStage = 0;
    GLuint m_fragmentStage = 0;
    GLuint m_geometryStage = 0;
    bool m_managed = true;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
