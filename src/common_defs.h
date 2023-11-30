#ifndef COMMON_DEFS_H
#define COMMON_DEFS_H

#define VIEW_PROJ_BINDING         0
#define DRAW_METADATA_BINDING     1
#define MESH_BINDING              2
#define INDIRECT_BINDING          3
#define ATOMIC_COUNTER_BINDING    4
#define DIRECTIONAL_LIGHT_BINDING 5

#define WORK_GROUP_LOCAL_SIZE_X 256
#define WORK_GROUP_LOCAL_SIZE_Y 1
#define WORK_GROUP_LOCAL_SIZE_Z 1

#define MAX_DRAWS (1 << 24)

#ifdef __cplusplus

#include <glm/glm.hpp>
extern "C" {
#include <GL/glew.h>
}

struct MeshTextures
{
    GLuint64 diffuse;
};

struct Mesh
{
    GLuint numIndices;
    GLuint firstIndex;
    GLuint baseVertex;
    GLuint _1;
    glm::mat3x4 transformation;
    MeshTextures textures;
    bool alive;  // TODO: Proper GL type?
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
    glm::mat3x4 MT;
    MeshTextures textures;
};

struct DirectionalLightData
{
    glm::vec3 direction;
    GLfloat strength;
    glm::vec3 color;
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
    uint _1;
    mat3x4 transformation;
    MeshTextures textures;
    bool alive;
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
    mat3x4 MT;
    MeshTextures textures;
};

struct DirectionalLightData
{
    vec3 direction;
    float strength;
    vec3 color;
};

#endif  // __cplusplus

#endif  // COMMON_DEFS_H
