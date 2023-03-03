#pragma once

#include "Buffer.hpp"
#include "Handle.hpp"
#include "Model.hpp"
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

class Renderer
{
public:
    struct Specification
    {
        std::span<Handle<Buffer>> vertexBuffers;
        Handle<Buffer> indexBuffer;
        std::span<Handle<ShaderProgram>> programs;

    };

    struct Job
    {
        Handle<Model> model;
        GLuint instanceCount;
        std::span<glm::mat3x4> transformations;
    };

    Renderer(ResourceManager* mngr, const Specification& spec);
    ~Renderer();

    void render() const noexcept;
    void submit(const Job& job) const noexcept;

private:
    ResourceManager* m_mngr;
    std::vector<Handle<ShaderProgram>> m_programs;
    GLuint m_vao;
    mutable Handle<Buffer> m_drawIndirectBuffer;
    mutable Handle<Buffer> m_transformsBuffer;
    mutable std::vector<Job> m_jobs;
};

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
