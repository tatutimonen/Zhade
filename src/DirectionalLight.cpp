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
    const auto& direction = m_settings.direction;
    const auto strength = m_settings.strength;
    const float data[] = {
        color.r, color.g, color.b, strength,
        direction.x, direction.y, direction.z
    };
    m_uniformBuffer.update(0, data, sizeof(data));
}

//------------------------------------------------------------------------
