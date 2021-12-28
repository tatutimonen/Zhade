#pragma once

#include "DirectionalLight.hpp"

//------------------------------------------------------------------------

DirectionalLight::DirectionalLight(const Settings& settings, GLenum usage)
    : m_uniformBuffer{UniformBuffer("DirectionalLight", Constants::DIRECTIONAL_LIGHT_BINDING, sizeof(Settings), 1, usage)}
{
    set(settings);
}

//------------------------------------------------------------------------

void DirectionalLight::set(const Settings& settings) noexcept
{
    m_settings = settings;
    const auto& color = m_settings.color;
    const auto shininess = m_settings.shininess;
    const auto& direction = m_settings.direction;
    const auto strength = m_settings.strength;
    const void* data = std::vector<float>({
        color.r, color.g, color.b, shininess,
        direction.x, direction.y, direction.z, strength
    }).data();
    m_uniformBuffer.update(0, data, sizeof(Settings));
}

//------------------------------------------------------------------------

void DirectionalLight::update(const observed::CameraPosition& message) noexcept
{
    auto& halfway = m_settings.halfway;
    halfway = glm::normalize(m_settings.direction + message);
    const void* data = std::vector<float>({halfway.x, halfway.y, halfway.z}).data();
    m_uniformBuffer.update(offsetof(Settings, halfway), data, sizeof(glm::vec3));
}

//------------------------------------------------------------------------
