#pragma once

#include "UniformBuffer.hpp"
#include "constants.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <memory>
#include <vector>

//------------------------------------------------------------------------

class AmbientLight {
public:
    explicit AmbientLight(const glm::vec4& color = glm::vec4(1.0f), GLenum usage = GL_STATIC_DRAW);

    void set(const glm::vec4& color) noexcept;
    inline const glm::vec4& get() const noexcept { return m_color; }

private:
    glm::vec4 m_color;
    UniformBuffer m_uniformBuffer;
};

//------------------------------------------------------------------------
