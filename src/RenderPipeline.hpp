#pragma once

#include "common.hpp"

extern "C" {
#include <GLFW/glfw3.h>
}

#include <optional>
#include <string>
<<<<<<< HEAD
#include <utility>
=======
>>>>>>> 81d827c67dc9574fa9d21a928fb8df9ee17ca63f

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

struct RenderPipelineDescriptor
{
    fs::path vertPath;
    fs::path fragPath;
    std::optional<fs::path> geomPath = std::nullopt;
<<<<<<< HEAD
    bool managed = true;
=======
>>>>>>> 81d827c67dc9574fa9d21a928fb8df9ee17ca63f
};

//------------------------------------------------------------------------

class RenderPipeline
{
public:
<<<<<<< HEAD
    RenderPipeline() = default;
    explicit RenderPipeline(RenderPipelineDescriptor desc);
    ~RenderPipeline();

    RenderPipeline(const RenderPipeline&) = delete;
    RenderPipeline& operator=(const RenderPipeline&) = delete;
    RenderPipeline(RenderPipeline&& other) = delete;
    RenderPipeline& operator=(RenderPipeline&&) = delete;
=======
    explicit RenderPipeline(RenderPipelineDescriptor desc);
>>>>>>> 81d827c67dc9574fa9d21a928fb8df9ee17ca63f

    void bind() const noexcept { glBindProgramPipeline(m_name); }

private:
<<<<<<< HEAD
    void freeResources() const noexcept;
    [[nodiscard]] std::string readShaderFile(const fs::path& path) const noexcept;
    void validate() const noexcept;

    GLuint m_name = 0;
    GLuint m_vertexStage = 0;
    GLuint m_fragmentStage = 0;
    GLuint m_geometryStage = 0;
    bool m_managed = true;
=======
    std::string parseShaderFile(const fs::path& path) const noexcept;
    void validate() const noexcept;

    GLuint m_name;
    GLuint m_vertexStage;
    GLuint m_fragmentStage;
    GLuint m_geometryStage = 0;
>>>>>>> 81d827c67dc9574fa9d21a928fb8df9ee17ca63f
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
