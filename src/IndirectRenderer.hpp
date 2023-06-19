#pragma once

#include "Buffer.hpp"
#include "Framebuffer.hpp"
#include "Handle.hpp"
#include "ResourceManager.hpp"
#include "Scene.hpp"

#include <vector>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class IndirectRenderer
{
public:
    struct RenderPass
    {
        Handle<Framebuffer> framebuffer;
        GLbitfield clearMask;
    };

    IndirectRenderer(ResourceManager* mngr, Scene* scene);

    void render() const noexcept;

private:
    const Buffer* commandBuffer() const noexcept { return m_mngr->get(m_commandBuffer); }
    const Buffer* modelMatrixBuffer() const noexcept { return m_mngr->get(m_modelMatrixBuffer); }
    const Buffer* textureBuffer() const noexcept { return m_mngr->get(m_textureBuffer); }

    void processSceneGraph() const noexcept;

    ResourceManager* m_mngr;
    Scene* m_scene;
    GLuint m_vao;
    Handle<Buffer> m_commandBuffer;
    Handle<Buffer> m_modelMatrixBuffer;
    Handle<Buffer> m_textureBuffer;
    std::vector<RenderPass> m_extraPasses;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
