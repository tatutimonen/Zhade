#pragma once

#include <cstdlib>
#include <format>
#ifdef _WIN32
#include <new>
#endif
#include <stdexcept>

extern "C" {
#ifdef _WIN32
#include <malloc.h>
#endif
}

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

template<typename T>
class AlignedResource
{
public:
    AlignedResource(size_t size)
        : m_size{size}
    {
    #ifdef _WIN32
        m_data = static_cast<T*>(_aligned_malloc(sizeof(T) * m_size, alignof(T)));
    #else
        m_data = static_cast<T*>(std::aligned_alloc(alignof(T), sizeof(T) * m_size));
    #endif
        if (m_data == nullptr) [[unlikely]]
            throw std::bad_alloc();
    }

    ~AlignedResource()
    {
    #ifdef _WIN32
        _aligned_free(m_data);
    #else
        std::free(m_data);
    #endif
    }

    AlignedResource(const AlignedResource&) = delete;
    AlignedResource& operator=(const AlignedResource&) = delete;
    
    AlignedResource(AlignedResource&& other)
    {
        m_data = other.m_data;
        other.m_data = nullptr;
    }
    
    AlignedResource& operator=(AlignedResource&& other)
    {
        m_data = other.m_data;
        other.m_data = nullptr;
        return *this;
    }

    [[nodiscard]] T& at(size_t pos)
    {
        if (pos > m_size - 1) [[unlikely]]
            throw std::out_of_range(std::format("Attempted to access index {} of an array of size {}", pos, m_size));

        return m_data[pos];
    }

    [[nodiscard]] const T& at(size_t pos) const
    {
        if (pos > m_size - 1) [[unlikely]]
            throw std::out_of_range(std::format("Attempted to access index {} of an array of size {}", pos, m_size));

        return m_data[pos];
    }

    [[nodiscard]] T& operator[](size_t pos)
    {
        return at(pos);
    }

private:
    T* m_data;
    size_t m_size;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
