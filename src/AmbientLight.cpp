#include "AmbientLight.hpp"

//------------------------------------------------------------------------

AmbientLight::AmbientLight(const std::shared_ptr<UniformBuffer>& uniformBuffer,
    const Settings& settings)
    : m_uniformBuffer{uniformBuffer}
{
    set(settings);
}

//------------------------------------------------------------------------

void AmbientLight::set(const Settings& settings) noexcept
{
    m_settings = settings;
    const auto& ambient = m_settings.ambient;
    const void* data = std::vector<GLfloat>({ambient.r, ambient.g, ambient.b, m_settings.intensity}).data();
    m_uniformBuffer->update(0, data, sizeof(AmbientLight::Settings));
}

//------------------------------------------------------------------------

void AmbientLight::setAmbient(const glm::vec3& ambient) noexcept
{
    m_settings.ambient = ambient;
    const void* data = std::vector<float>({ambient.r, ambient.g, ambient.b}).data();
    m_uniformBuffer->update(offsetof(AmbientLight::Settings, ambient), data, sizeof(glm::vec3));
}

//------------------------------------------------------------------------

void AmbientLight::setIntensity(GLfloat intensity) noexcept
{
    m_settings.intensity = intensity;
    m_uniformBuffer->update(offsetof(AmbientLight::Settings, intensity), &m_settings.intensity, sizeof(GLfloat));
}

//------------------------------------------------------------------------
