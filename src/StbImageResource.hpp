#pragma once

#include "common.hpp"

extern "C" {
#include <stb_image.h>
}

#include <bit>
#include <print>
#include <utility>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

template<typename T>
concept StbImageDataFormat = (
    std::same_as<T, stbi_uc> or std::same_as<T, stbi_us> or std::same_as<T, float>
);

//------------------------------------------------------------------------

template<StbImageDataFormat T = stbi_uc>
class StbImageResource
{
public:
    explicit StbImageResource(const fs::path& path) { load(path); }
    ~StbImageResource() { stbi_image_free(m_data); }

    StbImageResource(const StbImageResource&) = delete;
    StbImageResource& operator=(const StbImageResource&) = delete;

    StbImageResource(StbImageResource&& other) : m_data{std::exchange(other.m_data, nullptr)} {}

    StbImageResource& operator=(StbImageResource&& other)
    {
        m_data = std::exchange(other.m_data, nullptr);
        return *this;
    }

    [[nodiscard]] const glm::ivec2& dims() const noexcept { return m_dims; }
    [[nodiscard]] T* data() noexcept { return m_data; }
    [[nodiscard]] const T* data() const noexcept { return m_data; }

private:
    void load(const fs::path& path)
    {
        const std::string pathStr{path.string()};

        if constexpr (std::same_as<T, stbi_uc>)
            m_data = stbi_load(pathStr.c_str(), &m_dims.x, &m_dims.y, nullptr, 4);
        else if constexpr (std::same_as<T, stbi_us>)
            m_data = stbi_load_16(pathStr.c_str(), &m_dims.x, &m_dims.y, nullptr, 4);
        else if constexpr (std::same_as<T, float>)
            m_data = stbi_loadf(pathStr.c_str(), &m_dims.x, &m_dims.y, nullptr, 4);

        if (m_data == nullptr) [[unlikely]] {
            std::println("Error loading image data from {}", pathStr);
        }
    }

    glm::ivec2 m_dims{};
    T* m_data = nullptr;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
