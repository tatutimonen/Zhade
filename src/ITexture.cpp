#include "ITexture.hpp"

//------------------------------------------------------------------------

const std::map<GLenum, GLenum> ITexture::s_glFormatToSoilFormat = {
    {GL_RGB, SOIL_LOAD_RGB},
    {GL_RGBA, SOIL_LOAD_RGBA}
};

//------------------------------------------------------------------------
