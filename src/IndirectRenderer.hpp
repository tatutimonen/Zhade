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

struct Buffers
{
    Handle<Buffer> commandBuffer;
    Handle<Buffer> transformBuffer;
    Handle<Buffer> textureBuffer;
};

struct RenderPass
{
    Handle<Framebuffer> framebuffer;
    GLbitfield clearMask;
};

//------------------------------------------------------------------------

class IndirectRenderer
{
public:
    IndirectRenderer(ResourceManager* mngr, Scene* scene);

    void render() const noexcept;

private:
    const Buffer* commandBuffer() const noexcept { return m_mngr->get(m_buffers.commandBuffer); }
    const Buffer* modelMatrixBuffer() const noexcept { return m_mngr->get(m_buffers.transformBuffer); }
    const Buffer* textureBuffer() const noexcept { return m_mngr->get(m_buffers.textureBuffer); }

    void processSceneGraph() const noexcept;

    ResourceManager* m_mngr;
    Scene* m_scene;
    GLuint m_vao;
    Buffers m_buffers;
    std::vector<RenderPass> m_extraPasses;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
