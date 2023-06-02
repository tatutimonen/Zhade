#pragma once

#include "Handle.hpp"
#include "Texture.hpp"
#include "common.hpp"

#include <glm/glm.hpp>
extern "C" {
#include <GL/glew.h>
}

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class ResourceManager;

//------------------------------------------------------------------------

class Framebuffer
{
public:
    struct Desc
    {
        glm::ivec2 dims;
        Texture::Desc texDesc;
        GLenum attachment;
    };

    Framebuffer() = default;
    Framebuffer(ResourceManager* mngr, Desc desc, ResourceManagement management);
    ~Framebuffer();

    Framebuffer(const Framebuffer&) = delete;
    Framebuffer& operator=(const Framebuffer&) = delete;
    Framebuffer(Framebuffer&&) = default;
    Framebuffer& operator=(Framebuffer&&) = default;

    [[nodiscard]] GLuint getName() const noexcept { return m_name; }

    void bind() const noexcept { glBindFramebuffer(GL_FRAMEBUFFER, m_name); }
    void freeResources() const noexcept;

private:
    const Texture* texture() const noexcept;

    ResourceManager* m_mngr{};
    GLuint m_name{};
    Handle<Texture> m_texture;
    ResourceManagement m_management{ResourceManagement::MANUAL};
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
