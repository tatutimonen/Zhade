#pragma once

#include "Constants.hpp"
#include "UniformBuffer.hpp"

#include <glm/glm.hpp>

#include <algorithm>
#include <memory>
#include <vector>

//------------------------------------------------------------------------

class AmbientLight {
public:
    struct Settings {
        glm::vec3 ambient = glm::vec3(1.0f);
        GLfloat intensity = 1.0f;
    };

    explicit AmbientLight(const Settings& settings = Settings(), GLenum usage = GL_STATIC_DRAW);

    void set(const Settings& settings) noexcept;
    void setAmbient(const glm::vec3& ambient) noexcept;
    void setIntensity(GLfloat intensity) noexcept;
    inline const Settings& get() const noexcept         { return m_settings; }
    inline const glm::vec3& getAmbient() const noexcept { return m_settings.ambient; }
    inline GLfloat getIntensity() const noexcept        { return m_settings.intensity; }

private:
    Settings m_settings;
    UniformBuffer m_uniformBuffer;
};

//------------------------------------------------------------------------
