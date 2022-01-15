#pragma once

#include "Framebuffer.hpp"
#include "Texture2D.hpp"

#include <glm/gtx/type_trait.hpp>

#include <memory>

//------------------------------------------------------------------------

class ShadowMap {
public:
    explicit ShadowMap(const glm::uvec2& dimensions = glm::uvec2(2048, 2048));

private:
    glm::uvec2 m_dimensions;
    Framebuffer m_framebuffer;
    std::shared_ptr<Texture2D> m_depthTexture;
};

//------------------------------------------------------------------------
