#pragma once

#include "ResourceManager.hpp"

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

    };

    IndirectRenderer(ResourceManager* mngr);

private:
    ResourceManager* m_mngr;
    GLuint m_vao;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
