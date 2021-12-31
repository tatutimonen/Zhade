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
    const void* data = std::vector<float>({color.r, color.g, color.b, color.a}).data();
    m_uniformBuffer.update(0, data, sizeof(glm::vec4));
}

//------------------------------------------------------------------------
