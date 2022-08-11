#pragma once

#include <stb_image.h>

#include <string_view>
#include <type_traits>

//------------------------------------------------------------------------

template<typename T>
concept IsValidStbImageDataFormat = (
    std::is_same_v<T, stbi_uc> || std::is_same_v<T, stbi_us> || std::is_same_v<T, float>
);

//------------------------------------------------------------------------
// RAII wrapper for selected stb_image functionality.

template<typename T = stbi_uc>
requires IsValidStbImageDataFormat<T>
class StbImageResource
{
public:
    StbImageResource(std::string_view filename)
    {
        stbi_set_flip_vertically_on_load(1);
        m_data = load(filename.data());
    }

    ~StbImageResource() { stbi_image_free(m_data); }

    inline int getWidth() const noexcept { return m_width; }
    inline int getHeight() const noexcept { return m_height; }
    inline T* mutData() const noexcept { return m_data; }
    inline const T* data() const noexcept { return m_data; }

private:
    inline T* load(const char* filename) noexcept
    {
        if constexpr (std::is_same_v<T, stbi_uc>)
            return stbi_load(filename, &m_width, &m_height, nullptr, 4);
        else if (std::is_same_v<T, stbi_us>)
            return stbi_load_16(filename, &m_width, &m_height, nullptr, 4);
        else
            return stbi_loadf(filename, &m_width, &m_height, nullptr, 4);
    }

    int m_width = 0;
    int m_height = 0;
    T* m_data = nullptr;
};

//------------------------------------------------------------------------
