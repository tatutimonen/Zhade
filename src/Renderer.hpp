#pragma once

#include "Buffer.hpp"
#include "Framebuffer.hpp"
#include "Handle.hpp"
#include "ResourceManager.hpp"
#include "Scene.hpp"

#include <utility>
#include <vector>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

struct RenderPass
{
    using Stage = uint8_t;
    enum : Stage
    {
        SHADOW,
        MAIN
    };

    Handle<Framebuffer> framebuffer;
    GLbitfield clearMask;
    Stage stage;
};

struct RendererDescriptor
{
    ResourceManager* mngr;
    Scene* scene;
    BufferDescriptor commandBufferDesc{.byteSize = KIB_BYTES*100, .usage = BufferUsage::INDIRECT};
    BufferDescriptor transformBufferDesc{.byteSize = GIB_BYTES/2, .usage = BufferUsage::STORAGE};
    BufferDescriptor textureBufferDesc{.byteSize = KIB_BYTES*100, .usage = BufferUsage::STORAGE};
};

//------------------------------------------------------------------------

class Renderer
{
public:
    explicit Renderer(RendererDescriptor desc);
    ~Renderer();

    void render() const noexcept;

    [[nodiscard]] const Buffer* commandBuffer() const noexcept { return m_mngr->get(m_commandBuffer); }
    [[nodiscard]] const Buffer* transformBuffer() const noexcept { return m_mngr->get(m_transformBuffer); }
    [[nodiscard]] const Buffer* textureBuffer() const noexcept { return m_mngr->get(m_textureBuffer); }
private:

    [[nodiscard]] GLsizei drawCount() const noexcept
    {
        return commandBuffer()->getByteSize() / sizeof(DrawElementsIndirectCommand);
    }

    void processSceneGraph() const noexcept;
    void populateBuffers(std::span<Handle<Mesh>> meshesSorted) const noexcept;
    void invalidateBuffers() const noexcept;
    void pushMeshDataToBuffers(const Mesh* mesh) const noexcept;

    ResourceManager* m_mngr;
    Scene* m_scene;
    GLuint m_vao;
    Handle<Buffer> m_commandBuffer;
    Handle<Buffer> m_transformBuffer;
    Handle<Buffer> m_textureBuffer;
    std::vector<RenderPass> m_extraPasses;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
