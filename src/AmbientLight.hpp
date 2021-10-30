#pragma once

#include "ShaderProgram.hpp"

#include <glm/glm.hpp>

#include <algorithm>
#include <memory>
#include <set>

//------------------------------------------------------------------------

class AmbientLight final {
public:
    explicit AmbientLight(const glm::vec3& ambient = glm::vec3(1.0f));

    void setAmbient(const glm::vec3& ambient) noexcept;
    inline const glm::vec3& getAmbient() const noexcept { return m_ambient; }

    void uploadAmbient(ShaderProgram& shaderProgram) const noexcept;

private:
    glm::vec3 m_ambient;
};

//------------------------------------------------------------------------
