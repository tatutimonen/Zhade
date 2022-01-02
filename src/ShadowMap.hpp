#pragma once

#include "Framebuffer.hpp"
#include "Texture2D.hpp"

#include <glm/gtx/type_trait.hpp>

#include <memory>

//------------------------------------------------------------------------

class ShadowMap {
public:
    ShadowMap(const glm::ivec2& dimensions = glm::ivec2(2048, 2048));

private:
    glm::ivec2 m_dimensions;
    Framebuffer m_framebuffer;
    std::shared_ptr<Texture2D> m_depthTexture;
};

//------------------------------------------------------------------------
