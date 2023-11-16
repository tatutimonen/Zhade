#ifndef COMMON_DEFS_H
#define COMMON_DEFS_H

#define CAMERA_BINDING         0
#define MODEL_BINDING          1
#define TEXTURE_BINDING        2
#define MESH_BINDING           3
#define INDIRECT_BINDING       4
#define ATOMIC_COUNTER_BINDING 5

#define WORK_GROUP_LOCAL_SIZE_X 16
#define WORK_GROUP_LOCAL_SIZE_Y 2
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
	glm::mat3x4 transformation;
    MeshTextures textures;
	bool dirty;
	bool alive;
};

struct DrawElementsIndirectCommand
{
    GLuint count;
    GLuint instanceCount;
    GLuint firstIndex;
    GLuint baseIndex;
    GLuint baseInstance;
};

#else

struct MeshTextures
{
    sampler2D diffuse;
};

struct Mesh
{
    uint numIndices;
    uint firstIndex;
    uint baseVertex;
    mat3x4 transformation;
    MeshTextures textures;
    bool dirty;
    bool alive;
};

struct DrawElementsIndirectCommand
{
    uint count;
    uint instanceCount;
    uint firstIndex;
    uint baseIndex;
    uint baseInstance;
};

#endif  // __cplusplus

#endif  // COMMON_DEFS_H