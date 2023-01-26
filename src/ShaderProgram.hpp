#pragma once

#include "Shader.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <robin_hood.h>

#include <map>
#include <optional>
#include <string>
#include <type_traits>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

template<typename T>
concept ValidGLShaderDataType = (
    std::same_as<T, GLint>
        || std::same_as<T, GLfloat>
        || std::same_as<T, glm::vec2>
        || std::same_as<T, glm::vec3>
        || std::same_as<T, glm::vec4>
        || std::same_as<T, glm::mat3>
        || std::same_as<T, glm::mat3x4>
        || std::same_as<T, glm::mat4>
);

//------------------------------------------------------------------------

class ShaderProgram
{
public:
    ShaderProgram(const Shader<GL_VERTEX_SHADER>& vertexShader, const Shader<GL_FRAGMENT_SHADER>& fragmentShader,
        const std::optional<Shader<GL_GEOMETRY_SHADER>>& geometryShader = std::nullopt);
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;
    ShaderProgram(ShaderProgram&&) = default;
    ShaderProgram& operator=(ShaderProgram&&) = default;

    [[nodiscard]] GLint getHandle() const noexcept { return m_handle; }
    [[nodiscard]] GLint getLinkStatus() const noexcept { return m_linkStatus; }
    [[nodiscard]] GLint getUniformLocation(const std::string& name) const noexcept;

    void use() const noexcept { glUseProgram(m_handle); }
    void unuse() const noexcept { glUseProgram(0); }

    template<ValidGLShaderDataType T>
    void setUniform(const std::string& name, const void* data) const noexcept
    {
        const GLint location = getUniformLocation(name);
        if constexpr (std::same_as<T, GLfloat>)
            glUniform1f(location, *static_cast<const GLfloat*>(data));
        else if (std::same_as<T, GLint>)
            glUniform1i(location, *static_cast<const GLint*>(data));
        else if (std::same_as<T, glm::vec2>)
            glUniform2fv(location, 1, static_cast<const GLfloat*>(data));
        else if (std::same_as<T, glm::vec3>)
            glUniform3fv(location, 1, static_cast<const GLfloat*>(data));
        else if (std::same_as<T, glm::vec4>)
            glUniform4fv(location, 1, static_cast<const GLfloat*>(data));
        else if (std::same_as<T, glm::mat3>)
            glUniformMatrix3fv(location, 1, GL_FALSE, static_cast<const GLfloat*>(data));
        else if (std::same_as<T, glm::mat3x4>)
            glUniformMatrix3x4fv(location, 1, GL_FALSE, static_cast<const GLfloat*>(data));
        else if (std::same_as<T, glm::mat4>)
            glUniformMatrix4fv(location, 1, GL_FALSE, static_cast<const GLfloat*>(data));
    }

private:    
    void link() const noexcept;
    void detachShaders() const noexcept;

    GLuint m_handle;
    mutable GLint m_linkStatus;
    mutable robin_hood::unordered_map<std::string, GLint> m_uniformLocationCache;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------