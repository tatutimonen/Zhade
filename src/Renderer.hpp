#pragma once

#include "Handle.hpp"
#include "ResourceManager.hpp"
#include "ShaderProgram.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <span>
#include <vector>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class Buffer;

//------------------------------------------------------------------------

struct Vertex
{
    glm::vec3 pos = glm::vec3();
    glm::vec3 nrm = glm::vec3();
    glm::vec2 tex = glm::vec2();
};

//------------------------------------------------------------------------
// As per: https://www.khronos.org/opengl/wiki/Vertex_Rendering#Indirect_rendering.

struct MultiDrawElementsIndirectCommand
{
    GLuint vertexCount;
    GLuint instanceCount;
    GLuint firstIndex;
    GLuint baseVertex;
    GLuint baseInstance;
};

//------------------------------------------------------------------------

class Renderer;
/*{
public:
    struct Specification
    {
        std::span<Handle<Buffer>> vertexBuffers;
        Handle<Buffer> indexBuffer;
        std::span<Handle<ShaderProgram>> programs;

    };

    Renderer(const Specification& spec, const ResourceManager* mngr);
    ~Renderer();

    void submit(, std::span<glm::mat3x4> transforms) const noexcept;

private:
    const ResourceManager* m_mngr;
    GLuint m_vao;
    std::vector<Handle<ShaderProgram>> m_programs;
};*/

// Some resources:
// https://www.opengl.org/wiki/Vertex_Specification_Best_Practices
// https://www.youtube.com/watch?v=-bCeNzgiJ8I&t=1920s
// https://youtu.be/K70QbvzB6II?t=552

// * sort or bucket visible objects *
// foreach( render target )      // framebuffer
// foreach( pass )               // depth, blending, etc. states
// foreach( material )           // shaders
// foreach( material instance )  // textures
// foreach( vertex format )      // vertex buffers (batched)
// foreach( object )
// {
//     writeUniformData( object );
//     glDrawElementsbaseVertex(
//         GL_TRIANGLES,
//         object->indexCount,
//         GL_UNSIGNED_SHORT,
//         object->indexDataOffset,
//         object->baseVertex );
//     );   
// }

// - should maybe keep track of programs, textures, materials
// - data structure holding key-value pairs where key is like, e.g., here:
//   https://realtimecollisiondetection.net/blog/?p=86
// - sort according to the keys before looping through it and making the draw calls
// - or maybe separate queues for different types of objects?

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
