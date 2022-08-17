#include "TextureView.hpp"

#include "TextureStorage.hpp"
#include "util.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

TextureView::TextureView(const StorageDetails& storageDetails)
    : m_storageDetails{storageDetails}
{
    glGenTextures(1, &m_handle);
    const auto underlying = m_storageDetails.underlying.lock();
    const auto& settings = underlying->getSettings();
    CHECK_GL_ERROR(glTextureView(
        m_handle,
        GL_TEXTURE_2D,
        underlying->getHandle(),
        settings.internalformat,
        0, settings.levels,
        m_storageDetails.offset, 1
    ));
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
