#pragma once

#include "Handle.hpp"
#include "Texture.hpp"
#include "common_defs.h"

#include <span>
#include <vector>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------
    
class ResourceManager;

struct ModelDescriptor
{
    uint32_t id;
    std::span<Mesh> meshes;
    std::vector<Handle<Texture>> textures;
    glm::mat3x4 transformation{1.0f};
    ResourceManager* mngr = nullptr;
    bool managed = true;
};

//------------------------------------------------------------------------

class Model
{
public:
    Model() = default;
    explicit Model(ModelDescriptor desc);
    ~Model();

    void freeResources() const noexcept;

private:
    uint32_t m_id = 0;
    mutable std::span<Mesh> m_meshes;
    mutable std::vector<Handle<Texture>> m_textures;
    glm::mat3x4 m_transformation;
    ResourceManager* m_mngr = nullptr;
    bool m_managed = true;

    friend class Scene;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
