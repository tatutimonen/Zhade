#pragma once

#include "common.hpp"

extern "C" {
#include <GLFW/glfw3.h>
}

#include <string>
#include <vector>

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

inline constexpr GLenum PipelineStage2GLShader[] {
    GL_VERTEX_SHADER,
    GL_FRAGMENT_SHADER,
    GL_GEOMETRY_SHADER
};

inline constexpr GLenum PipelineStage2GLShaderBit[] {
    GL_VERTEX_SHADER_BIT,
    GL_FRAGMENT_SHADER_BIT,
    GL_GEOMETRY_SHADER_BIT
};

struct PipelineDescriptor
{
    fs::path vertPath;
    fs::path fragPath;
    fs::path geomPath{};
    std::vector<std::string> headers{"/bindings.h"};
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
    [[nodiscard]] std::string readFileContents(const fs::path& path) const noexcept;
    [[nodiscard]] GLuint createShaderProgram(PipelineStage::Type stage, const std::string& shaderSource) const noexcept;

    void setupHeaders() const noexcept;
    void setupStageProgram(PipelineStage::Type stage, const fs::path& shaderPath) const noexcept;
    void validate() const noexcept;

    GLuint m_name = 0;
    mutable GLuint m_stages[PipelineStage::NUM_SUPPORTED_STAGES] = { 0 };
    std::vector<std::string> m_headers;
    bool m_managed = true;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
