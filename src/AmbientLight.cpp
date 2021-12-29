#include "AmbientLight.hpp"

//------------------------------------------------------------------------

AmbientLight::AmbientLight(const Settings& settings, GLenum usage)
    : m_uniformBuffer{UniformBuffer("AmbientLight", constants::AMBIENT_LIGHT_BINDING, sizeof(Settings), 1, usage)}
{
    set(settings);
}

//------------------------------------------------------------------------

void AmbientLight::set(const Settings& settings) noexcept
{
    m_settings = settings;
    const auto& ambient = m_settings.ambient;
    const void* data = std::vector<GLfloat>({ambient.r, ambient.g, ambient.b, m_settings.intensity}).data();
    m_uniformBuffer.update(0, data, sizeof(Settings));
}

//------------------------------------------------------------------------

void AmbientLight::setAmbient(const glm::vec3& ambient) noexcept
{
    m_settings.ambient = ambient;
    const void* data = std::vector<float>({ambient.r, ambient.g, ambient.b}).data();
    m_uniformBuffer.update(offsetof(Settings, ambient), data, sizeof(glm::vec3));
}

//------------------------------------------------------------------------

void AmbientLight::setIntensity(GLfloat intensity) noexcept
{
    m_settings.intensity = intensity;
    m_uniformBuffer.update(offsetof(Settings, intensity), &m_settings.intensity, sizeof(GLfloat));
}

//------------------------------------------------------------------------
