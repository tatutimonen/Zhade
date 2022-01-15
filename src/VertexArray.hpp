#pragma once

#include "App.hpp"

#include <GL/glew.h>

#include <algorithm>
#include <cstdint>
#include <initializer_list>
#include <vector>

//------------------------------------------------------------------------

class VertexArray {
public:
    struct FormatSpecifier {
        uint32_t location;
        int32_t sizeBytes;
        uint32_t type;
        bool normalized;
        uint32_t offset;
    };

    explicit VertexArray(std::initializer_list<FormatSpecifier> format);
    ~VertexArray();
    VertexArray(const VertexArray&) = default;
    VertexArray(VertexArray&&) = default;
    VertexArray& operator=(const VertexArray&) = default;
    VertexArray& operator=(VertexArray&&) = default;

    inline void bind() const noexcept
    {
        if (!bound())
        {
            glBindVertexArray(m_handle);
            App::getInstance().currentlyBoundVertexArray = m_handle;
        }
    }

private:
    inline bool bound() const noexcept
    {
        return m_handle == App::getInstance().currentlyBoundVertexArray;
    }

    uint32_t m_handle;
    mutable bool bound = false;
};

//------------------------------------------------------------------------
