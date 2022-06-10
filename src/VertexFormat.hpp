#pragma once

#include "Shader.hpp"

#include <GL/glew.h>

#include <initializer_list>
#include <string>
#include <vector>

//------------------------------------------------------------------------
// Adapted from https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Renderer/Buffer.h.

class VertexFormat {
public:
    struct Element {
        Shader::DataType type;
        std::size_t sizeBytes;
        std::ptrdiff_t offset;
        bool packed;

        Element(const Shader::DataType type, const bool packed = false)
            : type{type},
              sizeBytes{Shader::getDataTypeSizeBytes(type)},
              offset{0},
              packed{packed}
        {}
    };

    VertexFormat(std::initializer_list<Element> elems);

    const std::vector<Element>& getElements() const noexcept { return m_elements; }
    std::uint32_t getStride() const noexcept { return m_stride; }

private:
    void computeOffsetsAndStride() noexcept;

    std::vector<Element> m_elements;
    std::uint32_t m_stride;
};

//------------------------------------------------------------------------
