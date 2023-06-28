#pragma once

#include "common.hpp"

extern "C" {
#include <GLFW/glfw3.h>
}

#include <iostream>
#include <string>
#include <string_view>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

namespace PipelineStage
{
    using Type = uint8_t;
    enum : Type
    {
        VERTEX,
        FRAGMENT,
        GEOMETRY,
        NUM_SUPPORTED_STAGES
    };
}

struct RenderPipelineDescriptor
{
    fs::path vertPath;
    fs::path fragPath;
    fs::path geomPath = "";
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
    RenderPipeline(RenderPipeline&&) = delete;
    RenderPipeline& operator=(RenderPipeline&&) = delete;

    void bind() const noexcept { glBindProgramPipeline(m_name); }
    void freeResources() const noexcept;

private:
    void checkStageProgramLinkStatus(PipelineStage::Type stage) const noexcept;
    [[nodiscard]] std::string readShaderFile(const fs::path& path) const noexcept;
    void validate() const noexcept;

    GLuint m_name = 0;
    GLuint m_stages[PipelineStage::NUM_SUPPORTED_STAGES] = { 0 };
    bool m_managed = true;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
