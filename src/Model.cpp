#include "Model.hpp"

#include "ResourceManager.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Model::Model(ModelDescriptor desc)
    : m_meshes{desc.meshes},
      m_textures{desc.textures},
      m_mat{desc.mat},
      m_mngr{desc.mngr}
{}

//------------------------------------------------------------------------

Model::~Model()
{
    if (m_mngr == nullptr) freeResources();
}

//------------------------------------------------------------------------

void Model::freeResources()
{
    uint32_t totalRefCount = 0;
    for (Mesh& mesh : m_meshes) {
        totalRefCount += --mesh.refCount;
    }
    if (totalRefCount == 0) {
        for (const Handle<Texture>& texHandle : m_textures) {
            m_mngr->destroy(texHandle);
        }
    }
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
