#pragma once

#include "common.hpp"

#include <glm/glm.hpp>
extern "C" {
#include <stb_image.h>
}

#include <bit>
#include <format>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

template<typename T>
concept StbImageDataFormat = (
    std::same_as<T, stbi_uc> || std::same_as<T, stbi_us> || std::same_as<T, float>
);

//------------------------------------------------------------------------
// RAII wrapper for selected stb_image functionality.

template<StbImageDataFormat T = stbi_uc>
class StbImageResource
{
public:
    StbImageResource(const fs::path& path) { load(path); }
    ~StbImageResource() { stbi_image_free(m_data); }

    StbImageResource(const StbImageResource&) = delete;
    StbImageResource& operator=(const StbImageResource&) = delete;

    StbImageResource(StbImageResource&& other)
    {
        m_data = other.m_data;
        other.m_data = nullptr;
    }

    StbImageResource& operator=(StbImageResource&& other)
    {
        if (this == &other) [[unlikely]] return *this;
        m_data = other.m_data;
        other.m_data = nullptr;
        return *this;
    }

    [[nodiscard]] const glm::ivec2& getDims() const noexcept { return m_dims; }
    [[nodiscard]] T* data() noexcept { return m_data; }
    [[nodiscard]] const T* data() const noexcept { return m_data; }

private:
    void load(const fs::path& path)
    {
        const char* rawPath = path.string().c_str();

        if constexpr (std::same_as<T, stbi_uc>)
            m_data = stbi_load(rawPath, &m_dims.x, &m_dims.y, nullptr, 4);
        else if (std::same_as<T, stbi_us>)
            m_data = stbi_load_16(rawPath, &m_dims.x, &m_dims.y, nullptr, 4);
        else if (std::same_as<T, float>)
            m_data = stbi_loadf(rawPath, &m_dims.x, &m_dims.y, nullptr, 4);
        
        if (m_data == nullptr) [[unlikely]]
            throw std::runtime_error(std::format("Error loading data from '{}'", rawPath));
    }

    glm::ivec2 m_dims;
    T* m_data;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
