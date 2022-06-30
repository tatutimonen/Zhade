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

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <stb_image.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <array>
#include <iostream>
#include <memory>
#include <memory_resource>

//------------------------------------------------------------------------

struct Vertex : AttributeData {
    glm::vec3 pos = glm::vec3();
    glm::vec3 nrm = glm::vec3();
    glm::vec2 tex = glm::vec2();

    Vertex() = default;
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

int main()
{
    using namespace sm;

    const auto app = std::make_shared<App>();;
    app->init();

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

    const auto scene = aiImportFile((common::assetPath + "dragon.obj").c_str(), aiProcessPreset_TargetRealtime_Fast);

    auto mesh = scene->mMeshes[0];
    
    std::array<Vertex, 8192> buf;
    std::pmr::monotonic_buffer_resource rsrc{buf.data(), buf.size()};
    auto dragonVerts = std::pmr::vector<Vertex>(&rsrc);

    for (auto i = 0u; i < mesh->mNumVertices; ++i)
    {
        const auto& pos = mesh->mVertices[i];
        const auto& nrm = mesh->mNormals[i];
        dragonVerts.emplace_back(glm::vec3(pos.x, pos.y, pos.z), glm::vec3(nrm.x, nrm.y, nrm.z), glm::vec2());
    }

    std::array<GLuint, 8192> bufIndices;
    std::pmr::monotonic_buffer_resource rsrcIndices{bufIndices.data(), bufIndices.size()};
    auto dragonIndices = std::pmr::vector<GLuint>(&rsrcIndices);

    for (auto i = 0u; i < mesh->mNumFaces; ++i)
    {
        const auto faceIndices = mesh->mFaces[i].mIndices;
        dragonIndices.push_back(faceIndices[0]);
        dragonIndices.push_back(faceIndices[1]);
        dragonIndices.push_back(faceIndices[2]);
    }

    const auto vbo = Buffer<Vertex, GL_ARRAY_BUFFER>();
    const auto ebo = Buffer<GLuint, GL_ELEMENT_ARRAY_BUFFER>();

    auto quadVtxSpan = vbo.pushData(quadVerts, 4);
    auto quadIdxSpan = ebo.pushData(quadInds, 6);
    auto triVtxSpan = vbo.pushData(triVerts, 3);
    auto triIdxSpan = ebo.pushData(triInds, 3);
    auto dragonVtxSpan = vbo.pushData(dragonVerts.data(), mesh->mNumVertices);
    auto dragonIdxSpan = ebo.pushData(dragonIndices.data(), mesh->mNumFaces * 3);

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
    modelMatrices.push_back(glm::translate(glm::vec3(0.0f, 5.0f, 0.0f)));

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

    const auto ssbo = Buffer<glm::mat4, GL_SHADER_STORAGE_BUFFER>(1 << 10);
    auto x = ssbo.pushData(modelMatrices.data(), modelMatrices.size());
    ssbo.bindBase(constants::MODEL_BINDING);

    // Setup the indirect draw buffer and the draw IDs.

    const auto dibo = Buffer<MultiDrawElementsIndirectCommand, GL_DRAW_INDIRECT_BUFFER>(1 << 10);
    auto y = dibo.pushData(cmds.data(), cmds.size());

    // Setup textures for the quads.

    GLuint tex;
    glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &tex);
    glTextureParameteri(tex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(tex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height;

    stbi_set_flip_vertically_on_load(1);
    uint8_t* imageData = stbi_load((common::texturePath + "cataphract.jpg").c_str(), &width, &height, nullptr, 4);
    glTextureStorage3D(tex, 4, GL_RGBA8, width, height, 5);
    glTextureSubImage3D(tex, 0, 0, 0, 0, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    stbi_image_free(imageData);
    uint8_t* imageData2 = stbi_load((common::texturePath + "berserk.png").c_str(), &width, &height, nullptr, 4);
    glTextureSubImage3D(tex, 0, 0, 0, 1, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, imageData2);
    stbi_image_free(imageData2);
    uint8_t* imageData3 = stbi_load((common::texturePath + "longbowman.png").c_str(), &width, &height, nullptr, 4);
    glTextureSubImage3D(tex, 0, 0, 0, 2, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, imageData3);
    stbi_image_free(imageData3);
    uint8_t* imageData4 = stbi_load((common::texturePath + "jaguarwarrior.png").c_str(), &width, &height, nullptr, 4);
    glTextureSubImage3D(tex, 0, 0, 0, 3, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, imageData4);
    stbi_image_free(imageData4);
    uint32_t jade = 0x0;  // 0.54,      0.89,     0.63
    jade = jade | ((std::uint32_t)(0.54f * 256) << 0);
    jade = jade | ((std::uint32_t)(0.89f * 256) << 8);
    jade = jade | ((std::uint32_t)(0.63f * 256) << 16);
    uint32_t emerald = 0x0;  //  0.07568, 0.61424,  0.07568
    emerald |= (std::uint32_t)(0.07568f * 256) << 0;
    emerald |= (std::uint32_t)(0.61424f * 256) << 8;
    emerald |= (std::uint32_t)(0.07568f * 256) << 16;
    glTextureSubImage3D(tex, 0, 0, 0, 4, 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &jade);

    glBindTextureUnit(0, tex);
    glGenerateTextureMipmap(tex);

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
