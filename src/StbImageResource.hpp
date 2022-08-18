#pragma once

#include <stb_image.h>

#include <iostream>
#include <string_view>
#include <type_traits>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

template<typename T>
concept StbImageDataFormat = (
    std::is_same_v<T, stbi_uc> || std::is_same_v<T, stbi_us> || std::is_same_v<T, float>
);

//------------------------------------------------------------------------
// RAII wrapper for selected stb_image functionality.

template<StbImageDataFormat T = stbi_uc>
class StbImageResource
{
public:
    StbImageResource(std::string_view filename) { load(filename); }
    ~StbImageResource() { stbi_image_free(m_data); }

    [[nodiscard]] int32_t getWidth() const noexcept { return m_width; }
    [[nodiscard]] int32_t getHeight() const noexcept { return m_height; }
    [[nodiscard]] T* mutData() const noexcept { return m_data; }
    [[nodiscard]] const T* data() const noexcept { return m_data; }

    [[nodiscard]] bool isValid() const noexcept { return m_data != nullptr; }

    static void setFlipY() noexcept
    {
        static bool flip = true;
        stbi_set_flip_vertically_on_load(flip);
        flip = !flip;
    }

private:
    [[nodiscard]] void load(std::string_view filename) noexcept
    {
        if constexpr (std::is_same_v<T, stbi_uc>)
            m_data = stbi_load(filename.data(), &m_width, &m_height, nullptr, 4);
        else if (std::is_same_v<T, stbi_us>)
            m_data = stbi_load_16(filename.data(), &m_width, &m_height, nullptr, 4);
        else if (std::is_same_v<T, float>)
            m_data = stbi_loadf(filename.data(), &m_width, &m_height, nullptr, 4);
        
        if (!isValid())
            std::cerr << filename << " yielded invalid data!" << std::endl;
    }

    int32_t m_width = 0;
    int32_t m_height = 0;
    T* m_data = nullptr;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
