#include "App.hpp"
#include "AttributeData.hpp"
#include "Buffer.hpp"
#include "PerspectiveCamera.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "Texture2D.hpp"
#include "UniformBuffer.hpp"
#include "common.hpp"
#include "constants.hpp"
#include "util.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <memory>
#include <memory_resource>

//------------------------------------------------------------------------

struct Vertex : AttributeData {
    glm::vec3 pos;
    glm::vec3 nrm;
    glm::vec2 tex;

    Vertex(const glm::vec3 pos, const glm::vec3 nrm, const glm::vec2 tex) : pos{pos}, nrm{nrm}, tex{tex} {}

    VertexFormat getVertexFormat() const noexcept override
    {
        return {
            { Shader::DataType::Vec3f },
            { Shader::DataType::Vec3f },
            { Shader::DataType::Vec2f }
        };
    }
};

//------------------------------------------------------------------------

struct Material {
    glm::vec3 ambient  = glm::vec3(1.0f);
    float _1           = constants::STD_140_PAD_FLOAT;
    glm::vec3 diffuse  = glm::vec3(1.0f);
    float _2           = constants::STD_140_PAD_FLOAT;
    glm::vec3 specular = glm::vec3(1.0f);
    float shininess    = 0.4f;
};

//------------------------------------------------------------------------

std::int32_t main()
{
    using namespace sm;

    const auto app = std::make_shared<App>();;
    app->init();

    auto tex = Texture2D::makeDefault();
    tex->bind();

    auto vshader = Shader(GL_VERTEX_SHADER, "debug.vert");
    auto fshader = Shader(GL_FRAGMENT_SHADER, "debug.frag");
    auto shaderProgram = ShaderProgram(vshader, fshader);

    auto camera = PerspectiveCamera(app);

    static constexpr auto numQuads = 4;
    static constexpr auto numTris = 2;

    // Basic vertex data setup.

    const Vertex quadVerts[] = {
        { glm::vec3( 0.5f,  0.0f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
        { glm::vec3( 0.5f,  0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f,  0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
        { glm::vec3(-0.5f,  0.0f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) }
    };
    const GLuint quadInds[] = {
        0, 1, 2,
        2, 3, 0
    };

    const Vertex triVerts[] = {
        { glm::vec3( 0.5f,  0.0f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
        { glm::vec3( 0.0f,  0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.5f, 1.0f) },
        { glm::vec3(-0.5f,  0.0f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) }
    };
    const GLuint triInds[] = {
        0, 1, 2
    };

    const auto vbo = Buffer<Vertex>(GL_ARRAY_BUFFER);
    const auto ebo = Buffer<GLuint>(GL_ELEMENT_ARRAY_BUFFER);

    auto quadVtxSpan = vbo.pushData(quadVerts, 4);
    auto quadIdxSpan = ebo.pushData(quadInds, 6);
    auto triVtxSpan = vbo.pushData(triVerts, 3);
    auto triIdxSpan = ebo.pushData(triInds, 3);

    GLuint vao;
    glCreateVertexArrays(1, &vao);
    glVertexArrayVertexBuffer(vao, 0, vbo.getHandle(), 0, sizeof(Vertex));
    glVertexArrayElementBuffer(vao, ebo.getHandle());

    glEnableVertexArrayAttrib(vao, 0);
    glEnableVertexArrayAttrib(vao, 1);
    glEnableVertexArrayAttrib(vao, 2);

    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
    glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, nrm));
    glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, tex));

    glVertexArrayAttribBinding(vao, 0, 0);
    glVertexArrayAttribBinding(vao, 1, 0);
    glVertexArrayAttribBinding(vao, 2, 0);

    // Create render commands and gather model matrices.

    std::vector<glm::mat4> modelMatrices;
    std::vector<MultiDrawElementsIndirectCommand> cmds;

    for (auto i = 0u; i < numQuads; ++i)
    {
        modelMatrices.push_back(glm::translate(glm::vec3(0.0f, (float)i, 0.0f)));
    }
    for (auto i = 0u; i < numTris; ++i)
    {
        modelMatrices.push_back(glm::translate(glm::vec3((float)(i+1), 0.0f, 0.0f)));
    }

    // Quads.
    /*cmds.push_back({
        .vertexCount = static_cast<GLuint>(quadIdxSpan.size()),
        .instanceCount = numQuads,
        .firstIndex = 0,
        .baseVertex = 0,
        .baseInstance = 0
    });
    cmds.push_back({
        .vertexCount = static_cast<GLuint>(triVtxSpan.size()),
        .instanceCount = numTris,
        .firstIndex = static_cast<GLuint>(quadIdxSpan.size()),
        .baseVertex = static_cast<GLuint>(quadVtxSpan.size()),
        .baseInstance = numQuads
    });*/

    cmds.push_back({
        .vertexCount = 6,
        .instanceCount = numQuads,
        .firstIndex = 0,
        .baseVertex = 0,
        .baseInstance = 0
    });
    cmds.push_back({
        .vertexCount = 3,
        .instanceCount = numTris,
        .firstIndex = 6,
        .baseVertex = 4,
        .baseInstance = 4
    });

    // Upload the model matrices into an SSBO.

    const auto ssbo = Buffer<glm::mat4>(GL_SHADER_STORAGE_BUFFER, 1 << 10);
    auto x = ssbo.pushData(modelMatrices.data(), modelMatrices.size());
    ssbo.bindBase(constants::MODEL_BINDING);

    // Setup the indirect draw buffer and the draw IDs.

    const auto dibo = Buffer<MultiDrawElementsIndirectCommand>(GL_DRAW_INDIRECT_BUFFER, 1 << 10);
    auto y = dibo.pushData(cmds.data(), cmds.size());

    shaderProgram.use();
    glBindVertexArray(vao);
    dibo.bind();

    while (!glfwWindowShouldClose(app->getGlCtx()))
    {
        app->updateInternalTimes();
        
        glfwPollEvents();
        camera.tick();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, cmds.size(), 0);

        glfwSwapBuffers(app->getGlCtx());
    }

    glBindVertexArray(0);

    return 0;
}

//------------------------------------------------------------------------
