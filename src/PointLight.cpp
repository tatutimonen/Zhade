#include "PointLight.hpp"

//------------------------------------------------------------------------

PointLight::PointLight(const UniformBufferStorage& uniformBufferStorage, const Settings& settings)
    : m_uniformBufferStorage{uniformBufferStorage},
      m_settings{settings}
{
    const auto& color = m_settings.color;
    const auto& position = m_settings.position;
    const auto attenuationConstant = m_settings.attenuationConstant;
    const auto attenuationLinear = m_settings.attenuationLinear;
    const auto attenuationQuadratic = m_settings.attenuationQuadratic;
    const float data[] = {
        color.r, color.g, color.b, constants::STD_140_PAD_FLOAT,
        position.x, position.y, position.z, m_settings.shininess,
        m_settings.strength, attenuationConstant, attenuationLinear, attenuationQuadratic
    };
    m_uniformBufferStorage.update(0, data, sizeof(Settings));
}

//------------------------------------------------------------------------

void PointLight::setColor(const glm::vec3& color) noexcept
{
    m_settings.color = color;
    const float data[] = {color.r, color.g, color.b};
    m_uniformBufferStorage.update(offsetof(Settings, color), data, sizeof(glm::vec3));
}

//------------------------------------------------------------------------

void PointLight::setPosition(const glm::vec3& position) noexcept
{
    m_settings.position = position;
    const float data[] = {position.x, position.y, position.z};
    m_uniformBufferStorage.update(offsetof(Settings, position), data, sizeof(glm::vec3));
}

//------------------------------------------------------------------------
