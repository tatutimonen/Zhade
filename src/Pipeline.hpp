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

struct PipelineDescriptor
{
    fs::path vertPath;
    fs::path fragPath;
    fs::path geomPath = "";
    bool managed = true;
};

//------------------------------------------------------------------------

class Pipeline
{
public:
    Pipeline() = default;
    explicit Pipeline(PipelineDescriptor desc);
    ~Pipeline();

    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;
    Pipeline(Pipeline&&) = delete;
    Pipeline& operator=(Pipeline&&) = delete;

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
