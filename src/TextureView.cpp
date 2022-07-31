#include "TextureView.hpp"

//------------------------------------------------------------------------

TextureView::TextureView(const StorageDetails& storageDetails)
    : m_storageDetails{storageDetails}
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_handle);
}

//------------------------------------------------------------------------
