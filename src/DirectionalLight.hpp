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
        glm::vec3 color     = glm::vec3(1.0f, 1.0f, 204.0f/255.0f);
        float strength      = 0.2f;
        glm::vec3 direction = glm::normalize(glm::vec3(0.5f, 0.5f, -0.6f));
        float _1            = constants::STD_140_PAD_FLOAT;
    };

    explicit DirectionalLight(const Settings& = Settings(), GLenum usage = GL_DYNAMIC_DRAW);

    void set(const Settings& settings) noexcept;

private:
    Settings m_settings;
    UniformBuffer m_uniformBuffer;
};

//------------------------------------------------------------------------
