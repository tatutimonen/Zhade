#ifndef COMMON_DEFS_H
#define COMMON_DEFS_H

#define VIEW_PROJ_BINDING                       0
#define HALF_VECTOR_BINDING                     1
#define DRAW_METADATA_BINDING                   2
#define MESH_BINDING                            3
#define INDIRECT_BINDING                        4
#define ATOMIC_COUNTER_BINDING                  5
#define DIRECTIONAL_LIGHT_PROPS_BINDING         6
#define DIRECTIONAL_LIGHT_DEPTH_TEXTURE_BINDING 7

#define WORK_GROUP_LOCAL_SIZE_X 256
#define WORK_GROUP_LOCAL_SIZE_Y   1
#define WORK_GROUP_LOCAL_SIZE_Z   1

#define MAX_DRAWS (1 << 24)
#define MAX_LIGHTS (1 + 8)  // Sun + point lights.

#ifdef __cplusplus

#include <glm/glm.hpp>
extern "C" {
#include <GL/glew.h>
}

#include <atomic>

struct MeshTextures
{
    GLuint64 diffuse;
};

struct Mesh
{
    GLuint numIndices;
    GLuint firstIndex;
    GLuint baseVertex;
    std::atomic_uint32_t refCount = 1;
    glm::mat3x4 modelMatT;
    MeshTextures textures;
};

struct DrawElementsIndirectCommand
{
    GLuint count;
    GLuint instanceCount;
    GLuint firstIndex;
    GLuint baseVertex;
    GLuint baseInstance;
};

struct DrawMetadata
{
    glm::mat3x4 modelMatT;
    glm::mat3 normalMat;
    MeshTextures textures;
};

struct DirectionalLightProperties
{
    glm::vec3 direction;
    GLfloat strength;
    glm::vec3 color;
    GLfloat _1;
    glm::vec3 ambient;
};

struct ViewProjMatrices
{
    glm::mat3x4 viewMatT;
    glm::mat4 projMat;
};

#else

struct MeshTextures
{
    uint64_t diffuse;
};

struct Mesh
{
    uint numIndices;
    uint firstIndex;
    uint baseVertex;
    uint refCount;
    mat3x4 modelMatT;
    MeshTextures textures;
};

struct DrawElementsIndirectCommand
{
    uint count;
    uint instanceCount;
    uint firstIndex;
    uint baseVertex;
    uint baseInstance;
};

struct DrawMetadata
{
    mat3x4 modelMatT;
    mat3 normalMat;
    MeshTextures textures;
};

struct DirectionalLightProperties
{
    vec3 direction;
    float strength;
    vec3 color;
    float _1;
    vec3 ambient;
};

struct ViewProjMatrices
{
    mat3x4 viewMatT;
    mat4 projMat;
};

#endif  // __cplusplus

#endif  // COMMON_DEFS_H
