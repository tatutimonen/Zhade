#pragma once

#include "Constants.hpp"
#include "Mesh.hpp"
#include "ShaderProgram.hpp"
#include "UniformBuffer.hpp"
#include "UniformBufferStorage.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <memory>
#include <vector>

//------------------------------------------------------------------------

class PointLight {
public:
    struct Settings {
        glm::vec3 color              = glm::vec3(1.0f);
        GLfloat _1                   = Constants::STD_140_PAD_FLOAT;
        glm::vec3 position           = glm::vec3(0.0f, 3.0f, 0.0f);
        GLfloat shininess            = 20.0f;
        GLfloat strength             = 10.0f;
        // Attenuation defaults: https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
        GLfloat attenuationConstant  = 1.0f;
        GLfloat attenuationLinear    = 0.35f;
        GLfloat attenuationQuadratic = 0.44f;
    };

    explicit PointLight(const UniformBufferStorage& uniformBufferStorage, const Settings& settings = Settings());

    void setColor(const glm::vec3& color) noexcept;
    void setPosition(const glm::vec3& position) noexcept;
    inline const glm::vec3& getColor() const noexcept    { return m_settings.color; }
    inline const glm::vec3& getPosition() const noexcept { return m_settings.position; }

private:
    UniformBufferStorage m_uniformBufferStorage;
    Settings m_settings;
};

//------------------------------------------------------------------------
