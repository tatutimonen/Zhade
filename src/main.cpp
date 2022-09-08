#include "App.hpp"
#include "Buffer.hpp"
#include "PerspectiveCamera.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "TextureStorage.hpp"
#include "TextureView.hpp"
#include "common.hpp"
#include "constants.hpp"
#include "StbImageResource.hpp"
#include "util.hpp"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <array>
#include <iostream>
#include <memory>
#include <vector>

//------------------------------------------------------------------------

struct Vertex
{
    glm::vec3 pos = glm::vec3();
    glm::vec3 nrm = glm::vec3();
    glm::vec2 tex = glm::vec2();
};

//------------------------------------------------------------------------

int main()
{
    using namespace Zhade;

    const auto app = App();
    app.init();

    auto shaderProgram = ShaderProgram(
        Shader<GL_VERTEX_SHADER>(common::shaderPath + "debug.vert"),
        Shader<GL_FRAGMENT_SHADER>(common::shaderPath + "debug.frag")
    );

    auto camera = PerspectiveCamera(app);

    static constexpr auto numQuads = 4;
    static constexpr auto numTris = 2;

    auto test = TextureStorage<GL_DEPTH_COMPONENT32, float>::Settings::makeDefault(glm::ivec4(1, 1, 1, 1));

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

    const auto scene = aiImportFile((common::assetPath + "dragon.obj").c_str(), aiProcessPreset_TargetRealtime_Fast);

    auto mesh = scene->mMeshes[0];
    
    auto dragonVerts = std::vector<Vertex>();

    for (auto i = 0u; i < mesh->mNumVertices; ++i)
    {
        const auto& pos = mesh->mVertices[i];
        const auto& nrm = mesh->mNormals[i];
        dragonVerts.push_back({ glm::vec3(pos.x, pos.y, pos.z), glm::vec3(nrm.x, nrm.y, nrm.z), glm::vec2() });
    }

    auto dragonIndices = std::vector<GLuint>();

    for (auto i = 0u; i < mesh->mNumFaces; ++i)
    {
        const auto faceIndices = mesh->mFaces[i].mIndices;
        dragonIndices.push_back(faceIndices[0]);
        dragonIndices.push_back(faceIndices[1]);
        dragonIndices.push_back(faceIndices[2]);
    }

    const auto vbo = Buffer<Vertex, GL_ARRAY_BUFFER>();
    const auto ebo = Buffer<GLuint, GL_ELEMENT_ARRAY_BUFFER>();
    const auto vbo2 = Buffer<Vertex, GL_ARRAY_BUFFER>(64);

    auto quadVtxSpan = vbo.pushData(quadVerts, 4);
    auto quadIdxSpan = ebo.pushData(quadInds, 6);
    auto triVtxSpan = vbo.pushData(triVerts, 3);
    auto triIdxSpan = ebo.pushData(triInds, 3);
    auto dragonVtxSpan = vbo.pushData(dragonVerts.data(), mesh->mNumVertices);
    auto dragonIdxSpan = ebo.pushData(dragonIndices.data(), mesh->mNumFaces * 3);

    GLuint vao;
    glCreateVertexArrays(1, &vao);
    glVertexArrayVertexBuffer(vao, 1, vbo.getHandle(), 0, sizeof(Vertex));
    glVertexArrayVertexBuffer(vao, 0, vbo2.getHandle(), 0, sizeof(Vertex));
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
    glVertexArrayAttribBinding(vao, 0, 1);
    glVertexArrayAttribBinding(vao, 1, 1);
    glVertexArrayAttribBinding(vao, 2, 1);

    // Create render commands and gather model matrices.

    std::vector<glm::mat3x4> modelMatrices;
    std::vector<MultiDrawElementsIndirectCommand> cmds;

    for (auto i = 0u; i < numQuads; ++i)
    {
        modelMatrices.push_back(glm::mat3x4(glm::transpose(glm::translate(glm::vec3(0.0f, (float)i, 0.0f)))));
    }
    for (auto i = 0u; i < numTris; ++i)
    {
        modelMatrices.push_back(glm::mat3x4(glm::transpose(glm::translate(glm::vec3((float)(i+1), 0.0f, 0.0f)))));
    }
    modelMatrices.push_back(glm::mat3x4(glm::transpose(glm::translate(glm::vec3(0.0f, 5.0f, 0.0f)))));

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
    cmds.push_back({
        .vertexCount = mesh->mNumFaces * 3,
        .instanceCount = 1,
        .firstIndex = 9,
        .baseVertex = 7,
        .baseInstance = numQuads + numTris
    });

    // Upload the model matrices into an SSBO.

    const auto ssbo = Buffer<glm::mat3x4, GL_SHADER_STORAGE_BUFFER>(1 << 10);
    auto x = ssbo.pushData(modelMatrices.data(), modelMatrices.size());
    ssbo.bindBase(constants::MODEL_BINDING);

    // Setup the indirect draw buffer and the draw IDs.

    const auto dibo = Buffer<MultiDrawElementsIndirectCommand, GL_DRAW_INDIRECT_BUFFER>(1 << 10);
    auto y = dibo.pushData(cmds.data(), cmds.size());

    // Setup textures for the quads.

    auto texStorage = TextureStorage();

    auto cataView = texStorage.pushDataFromFile(common::texturePath + "cataphract.jpg").value();
    auto berserkView = texStorage.pushDataFromFile(common::texturePath + "berserk.png").value();
    auto longbowView = texStorage.pushDataFromFile(common::texturePath + "longbowman.png").value();
    auto jagView = texStorage.pushDataFromFile(common::texturePath + "jaguarwarrior.png").value();

    /*auto materialStorage = std::make_shared<TextureStorage>(1, 1, 1, 1);
    uint32_t jade = 0x0;  // 0.54,      0.89,     0.63
    jade = jade | ((uint32_t)(0.54f * 256) << 0);
    jade = jade | ((uint32_t)(0.89f * 256) << 8);
    jade = jade | ((uint32_t)(0.63f * 256) << 16);
    // Two triangles after the four quads.
    auto jadeView = materialStorage->pushData(&jade);*/

    const auto img = StbImageResource(common::texturePath + "berserk.png");
    glTextureSubImage2D(
        jagView.getHandle(), 0,
        0, 0, 256, 256,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        img.data()
    );
    glGenerateTextureMipmap(jagView.getHandle());

    texStorage.bindToUnit(0);
    texStorage.generateMipmap();

    shaderProgram.use();
    glBindVertexArray(vao);
    dibo.bind();

    while (!glfwWindowShouldClose(app.getGLCtx()))
    {
        app.updateInternalTimes();
        
        glfwPollEvents();
        camera.tick();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, cmds.size(), 0);

        glfwSwapBuffers(app.getGLCtx());
    }

    glBindVertexArray(0);

    return 0;
}

//------------------------------------------------------------------------
