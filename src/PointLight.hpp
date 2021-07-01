#pragma once

#include "Mesh.hpp"
#include "ShaderProgram.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <memory>


class PointLight final : public Mesh {
public:
    struct Specification {
        GLfloat shininess            = 20.0f;
        GLfloat strength             = 10.0f;
        // Attenuation defaults: https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
        GLfloat attenuationConstant  = 1.0f;
        GLfloat attenuationLinear    = 0.35f;
        GLfloat attenuationQuadratic = 0.44f;

        Specification() = default;
    };

    PointLight(std::shared_ptr<Mesh> mesh,
               const Specification& specification = Specification());

private:
    Specification mSpecification;
};