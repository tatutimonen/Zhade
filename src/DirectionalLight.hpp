#pragma once

#include "constants.hpp"
#include "UniformBuffer.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>

//------------------------------------------------------------------------

class DirectionalLight {
public:
    struct Settings {
        glm::vec3 color     = glm::vec3(0.5f);
        float shininess     = 100.0f;
        glm::vec3 direction = glm::normalize(glm::vec3(1.0f, 3.0f, 0.0f));
        float strength      = 50.0f;
    };

    explicit DirectionalLight(const Settings& = Settings(), GLenum usage = GL_DYNAMIC_DRAW);

    void set(const Settings& settings) noexcept;

private:
    Settings m_settings;
    UniformBuffer m_uniformBuffer;
};

//------------------------------------------------------------------------
