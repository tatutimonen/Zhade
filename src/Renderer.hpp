#pragma once

#include "Buffer.hpp"
#include "Handle.hpp"
#include "Model.hpp"
#include "ResourceManager.hpp"
#include "ShaderProgram.hpp"
#include "Texture.hpp"

#include <glm/glm.hpp>
extern "C" {
#include <GL/glew.h>
}

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
        Handle<Buffer> vertexBuffer;
        Handle<Buffer> indexBuffer;
        ShaderProgram* program;
    };

    struct Task
    {
        Handle<Model> model;
        GLuint instanceCount;
        std::span<glm::mat3x4> transformations;
        std::span<Texture> textures;
    };

    Renderer(ResourceManager* mngr, const Specification& spec);
    ~Renderer();

    void render() const noexcept;
    void submit(const Task& task) const noexcept;

private:
    ResourceManager* m_mngr;
    ShaderProgram* m_program;
    GLuint m_vao;
    mutable Handle<Buffer> m_vertexBuffer;
    mutable Handle<Buffer> m_indexBuffer;
    mutable Handle<Buffer> m_drawIndirectBuffer;
    mutable Handle<Buffer> m_transformsBuffer;
    mutable Handle<Buffer> m_textureBuffer;
    mutable std::vector<Task> m_tasks;

    MultiDrawElementsIndirectCommand* cmdData;
    glm::mat3x4* transformsData;
    GLuint64* textureData;
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
