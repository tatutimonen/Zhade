#pragma once

#include "Buffer.hpp"
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
        GLuint fbo = 0;
    };

    IndirectRenderer(ResourceManager* mngr, Scene* scene);

    void render() const noexcept;

private:
    const Buffer* commandBuffer() const noexcept { return m_mngr->get(m_commandBuffer); }

    void setupCommandBuffer() const noexcept;

    ResourceManager* m_mngr;
    Scene* m_scene;
    GLuint m_vao;
    Handle<Buffer> m_commandBuffer;
    Handle<Buffer> m_transformBuffer;
    Handle<Buffer> m_textureBuffer;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
