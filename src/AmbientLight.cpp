#include "AmbientLight.hpp"

//------------------------------------------------------------------------

AmbientLight::AmbientLight(const glm::vec4& color, GLenum usage)
    : m_uniformBuffer{UniformBuffer("AmbientLight", constants::AMBIENT_LIGHT_BINDING, sizeof(glm::vec4), 1, usage)}
{
    set(color);
}

//------------------------------------------------------------------------

void AmbientLight::set(const glm::vec4& color) noexcept
{
    m_color = color;
    m_uniformBuffer.update(0, glm::value_ptr(m_color), sizeof(glm::vec4));
}

//------------------------------------------------------------------------
