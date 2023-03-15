#pragma once

#include <cstdint>
#include <format>
#include <string_view>

extern "C" {
#include <stb_image.h>
}

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
    StbImageResource(std::string_view filename) { load(filename); }
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

    [[nodiscard]] int32_t getWidth() const noexcept { return m_width; }
    [[nodiscard]] int32_t getHeight() const noexcept { return m_height; }
    [[nodiscard]] T* data() noexcept { return m_data; }
    [[nodiscard]] const T* data() const noexcept { return m_data; }

    static void setGlobalFlipY(bool flip) noexcept { stbi_set_flip_vertically_on_load(flip); }

private:
    void load(std::string_view filename)
    {
        if constexpr (std::same_as<T, stbi_uc>)
            m_data = stbi_load(filename.data(), &m_width, &m_height, nullptr, 4);
        else if (std::same_as<T, stbi_us>)
            m_data = stbi_load_16(filename.data(), &m_width, &m_height, nullptr, 4);
        else if (std::same_as<T, float>)
            m_data = stbi_loadf(filename.data(), &m_width, &m_height, nullptr, 4);
        
        if (m_data == nullptr) [[unlikely]]
            throw std::runtime_error(std::format("Error loading data from '{}'", filename));
    }

    int32_t m_width;
    int32_t m_height;
    T* m_data;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
