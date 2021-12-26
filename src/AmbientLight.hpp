#pragma once

#include "UniformBuffer.hpp"

#include <glm/glm.hpp>

#include <algorithm>
#include <memory>
#include <vector>

//------------------------------------------------------------------------

class AmbientLight final {
public:
    struct Settings {
        glm::vec3 ambient = glm::vec3(1.0f);
        GLfloat intensity = 1.0f;
    };

    explicit AmbientLight(const std::shared_ptr<UniformBuffer>& uniformBuffer,
        const Settings& settings = Settings());

    void set(const Settings& settings) noexcept;
    void setAmbient(const glm::vec3& ambient) noexcept;
    void setIntensity(GLfloat intensity) noexcept;
    inline const Settings& get() const noexcept         { return m_settings; }
    inline const glm::vec3& getAmbient() const noexcept { return m_settings.ambient; }
    inline GLfloat getIntensity() const noexcept        { return m_settings.intensity; }

private:
    std::shared_ptr<UniformBuffer> m_uniformBuffer;
    Settings m_settings;
};

//------------------------------------------------------------------------
