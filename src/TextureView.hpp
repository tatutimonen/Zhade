#pragma once

#include <GL/glew.h>

#include <memory>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class TextureStorage;

//------------------------------------------------------------------------

class TextureView
{
public:
    inline GLuint getHandle() const noexcept { return m_handle; }

private:
    struct StorageDetails
    {
        std::weak_ptr<const TextureStorage> underlying;
        const GLsizeiptr offset;
    };

    TextureView(const StorageDetails& storageDetails);

    GLuint m_handle;
    StorageDetails m_storageDetails;

    friend class TextureStorage;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
