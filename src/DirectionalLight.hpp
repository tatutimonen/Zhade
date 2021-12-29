#pragma once

#include "constants.hpp"
#include "Observer.hpp"
#include "UniformBuffer.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>

//------------------------------------------------------------------------

class DirectionalLight : public Observer<observed::CameraCenter> {
public:
    struct Settings {
        glm::vec3 color     = glm::vec3(1.0f);
        GLfloat shininess   = 20.0f;
        glm::vec3 direction = glm::rotate(-75.0f, glm::vec3(0.0f, 0.0f, 1.0f)) \
                              * glm::rotate(45.0f, glm::vec3(0.0f, 1.0f, 0.0f)) \
                              * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        GLfloat strength    = 10.0f;
        glm::vec3 halfway   = glm::vec3(0.0f);
        GLfloat _1          = constants::STD_140_PAD_FLOAT;
    };

    explicit DirectionalLight(const Settings& = Settings(), GLenum usage = GL_DYNAMIC_DRAW);

    void set(const Settings& settings) noexcept;

    virtual void update(const observed::CameraCenter& message) noexcept override;

private:
    Settings m_settings;
    UniformBuffer m_uniformBuffer;
};

//------------------------------------------------------------------------
