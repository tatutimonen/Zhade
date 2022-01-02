#pragma once

#include "App.hpp"
#include "Texture.hpp"
#include "Texture2D.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/type_trait.hpp>

#include <cstdint>
#include <memory>
#include <unordered_set>
#include <vector>

//------------------------------------------------------------------------

class Framebuffer {
public:
    Framebuffer(const glm::uvec2& dimensions);
    ~Framebuffer();

    void bind(uint32_t target = GL_FRAMEBUFFER) const noexcept;
    void unbind(uint32_t target = GL_FRAMEBUFFER) const noexcept;

    void attachTexture2D(std::shared_ptr<const Texture2D> texture, uint32_t attachment) noexcept;

private:
    uint32_t m_handle;
    glm::uvec2 m_dimensions;
    std::vector<std::shared_ptr<const Texture>> m_textureAttachments;
    std::unordered_set<uint32_t> m_textureAttachmentMemo;
};

//------------------------------------------------------------------------
