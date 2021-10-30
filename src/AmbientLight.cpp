#include "AmbientLight.hpp"

//------------------------------------------------------------------------

AmbientLight::AmbientLight(const glm::vec3& ambient)
    : m_ambient{ambient}
{}

//------------------------------------------------------------------------

void AmbientLight::setAmbient(const glm::vec3& ambient) noexcept
{
    m_ambient = ambient;
}

//------------------------------------------------------------------------

void AmbientLight::uploadAmbient(ShaderProgram& shaderProgram) const noexcept
{
    shaderProgram.setUniform<glm::vec3>("u_ambient", glm::value_ptr(m_ambient));
}

//------------------------------------------------------------------------
