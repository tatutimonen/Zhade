#pragma once

#include "Mesh.hpp"
#include "ShaderProgram.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <memory>
#include <vector>

//------------------------------------------------------------------------

class PointLight final {
public:
    struct Settings {
        glm::vec3 color              = glm::vec3(1.0f);
        glm::vec3 position           = glm::vec3(0.0f, 3.0f, 0.0f);
        GLfloat shininess            = 20.0f;
        GLfloat strength             = 10.0f;
        // Attenuation defaults: https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
        GLfloat attenuationConstant  = 1.0f;
        GLfloat attenuationLinear    = 0.35f;
        GLfloat attenuationQuadratic = 0.44f;
    };

    PointLight(const Settings& settings = Settings());

    inline void setColor(const glm::vec3& color) { m_settings.color = color; }

private:
    Settings m_settings;
};

//------------------------------------------------------------------------
