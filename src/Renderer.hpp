#pragma once

#include "Buffer.hpp"
#include "Framebuffer.hpp"
#include "Handle.hpp"
#include "ResourceManager.hpp"
#include "Scene.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

struct RendererDescriptor
{
    ResourceManager* mngr;
    Scene* scene;
    BufferDescriptor commandBufferDesc{
        .byteSize = KIB_BYTES * 100,
        .usage = BufferUsage::INDIRECT
    };
    BufferDescriptor transformBufferDesc{
        .byteSize = MAX_MODELS * sizeof(glm::mat3x4),
        .usage = BufferUsage::STORAGE
    };
    BufferDescriptor drawID2ModelBufferDesc{
        .byteSize = MAX_MODELS * sizeof(uint32_t),
        .usage = BufferUsage::STORAGE
    };
    BufferDescriptor textureBufferDesc{
        .byteSize = KIB_BYTES * 100,
        .usage = BufferUsage::STORAGE
    };
};

//------------------------------------------------------------------------

class Renderer
{
public:
    explicit Renderer(RendererDescriptor desc);
    ~Renderer();

    void render() const noexcept;
    void processSceneGraph() const noexcept;

private:
    [[nodiscard]] const Buffer* commandBuffer() const noexcept { return m_mngr->get(m_commandBuffer); }
    [[nodiscard]] const Buffer* transformBuffer() const noexcept { return m_mngr->get(m_transformBuffer); }
    [[nodiscard]] const Buffer* drawID2ModelBuffer() const noexcept { return m_mngr->get(m_drawID2ModelBuffer); }
    [[nodiscard]] const Buffer* textureBuffer() const noexcept { return m_mngr->get(m_textureBuffer); }

    [[nodiscard]] GLsizei drawCount() const noexcept
    {
        return commandBuffer()->byteSize() / sizeof(DrawElementsIndirectCommand);
    }

    void populateBuffers() const noexcept;
    void invalidateBuffers() const noexcept;
    void pushMeshDataToBuffers(const Mesh* mesh) const noexcept;

    ResourceManager* m_mngr;
    Scene* m_scene;
    GLuint m_vao;
    Handle<Buffer> m_commandBuffer;
    Handle<Buffer> m_transformBuffer;
    Handle<Buffer> m_drawID2ModelBuffer;
    Handle<Buffer> m_textureBuffer;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
