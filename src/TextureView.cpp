#include "TextureView.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

TextureView::TextureView(const StorageDetails& storageDetails)
    : m_storageDetails{storageDetails}
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_handle);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
