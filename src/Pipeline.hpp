#pragma once

#include "common.hpp"

#include <absl/types/span.h>

#include <array>
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
        COMPUTE,
        NUM_SUPPORTED_STAGES
    };
}

inline constexpr GLenum PipelineStage2GLShader[] {
    GL_VERTEX_SHADER,
    GL_FRAGMENT_SHADER,
    GL_GEOMETRY_SHADER,
    GL_COMPUTE_SHADER
};

inline constexpr GLenum PipelineStage2GLShaderBit[] {
    GL_VERTEX_SHADER_BIT,
    GL_FRAGMENT_SHADER_BIT,
    GL_GEOMETRY_SHADER_BIT,
    GL_COMPUTE_SHADER_BIT
};

struct PipelineDescriptor
{
    fs::path vertPath;
    fs::path fragPath;
    fs::path geomPath{};
    fs::path compPath{};
    absl::Span<const std::string_view> headers{"/common_defs.h"};
    bool managed = true;
};

//------------------------------------------------------------------------

class Pipeline
{
public:
    Pipeline() = default;
    explicit Pipeline(PipelineDescriptor desc);
    ~Pipeline();

    Pipeline(const Pipeline&) = default;
    Pipeline& operator=(const Pipeline&) = default;
    Pipeline(Pipeline&&) = delete;
    Pipeline& operator=(Pipeline&&) = delete;

    void bind() { glBindProgramPipeline(m_name); }
    void freeResources();

private:
    [[nodiscard]] std::string readFileContents(const fs::path& path);
    [[nodiscard]] GLuint createShaderProgramInclude(PipelineStage::Type stage, const fs::path& shaderPath);

    void setupHeaders();
    void setupStageProgram(PipelineStage::Type stage, const fs::path& shaderPath);
    void validate();

    GLuint m_name = 0;
    std::array<GLuint, PipelineStage::NUM_SUPPORTED_STAGES> m_stages;
    std::vector<std::string> m_headers;
    bool m_managed = true;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
