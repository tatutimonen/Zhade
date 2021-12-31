#pragma once

#include "DirectionalLight.hpp"

//------------------------------------------------------------------------

DirectionalLight::DirectionalLight(const Settings& settings, GLenum usage)
    : m_uniformBuffer{UniformBuffer("DirectionalLight", constants::DIRECTIONAL_LIGHT_BINDING, sizeof(Settings), 1, usage)}
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
    const float data[8] = {
        color.r, color.g, color.b, shininess,
        direction.x, direction.y, direction.z, strength
    };
    m_uniformBuffer.update(0, data, sizeof(Settings));
}

//------------------------------------------------------------------------
