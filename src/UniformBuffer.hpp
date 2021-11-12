#pragma once

#include "ShaderProgram.hpp"
#include "Util.hpp"

#include <GL/glew.h>

#include <algorithm>
#include <cstring>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

//------------------------------------------------------------------------

class UniformBuffer {
public:
    struct Data {
        using Element = std::tuple<std::string, GLvoid*, GLenum>;  // (name, data, type)

        Data() = default;
        explicit Data(const std::vector<Element>& elements)
            : elements{std::move(elements)}
        {}

        std::vector<GLchar*> getNames() const noexcept
        {
            std::vector<GLchar*> names;
            std::transform(elements.cbegin(),
                elements.cend(),
                names.begin(),
                [](const auto& element)
                {
                    return std::get<0>(element).data();
                });
            return names;
        }

        inline uint32_t size() const noexcept
        {
            return elements.size();
        }

        std::vector<Element> elements;
    };

    UniformBuffer(const std::string& blockName,
        GLuint binding,
        const std::shared_ptr<ShaderProgram>& shaderProgram,
        const Data& data = Data());
    ~UniformBuffer();



private:
    GLuint m_handle;
    GLuint m_uniformBlockIdx;
    std::vector<GLuint> m_indices;
    std::vector<GLint> m_offset;
    std::vector<GLint> m_size;
    std::vector<GLint> m_type;

    std::shared_ptr<ShaderProgram> m_shaderProgram;
};

//------------------------------------------------------------------------
