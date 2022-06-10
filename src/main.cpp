#include "App.hpp"
#include "AttributeData.hpp"
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

//------------------------------------------------------------------------

struct Vertex : AttributeData {
    glm::vec3 pos;
    glm::vec3 nrm;
    glm::vec2 tex;

    Vertex(const glm::vec3& pos, const glm::vec3& nrm, const glm::vec2& tex) : pos{pos}, nrm{nrm}, tex{tex} {}

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

    Material() = default;
    Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess)
        : ambient{ambient},
          diffuse{diffuse},
          specular{specular},
          shininess{shininess}
    {}
};

//------------------------------------------------------------------------

std::int32_t main()
{
    const auto app = std::make_shared<App>();;
    app->init();

    static const std::vector<Vertex> vertices{
        { glm::vec3( 0.5f,  0.0f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
        { glm::vec3( 0.5f,  0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f,  0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
        { glm::vec3(-0.5f,  0.0f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) }
    };
    
    static const std::vector<std::uint32_t> indices{
        0, 1, 2, 2, 3, 0
    };

    auto tex = Texture2D::makeDefault();
    tex->bind();

    auto vshader = Shader(GL_VERTEX_SHADER, "debug.vert");
    auto fshader = Shader(GL_FRAGMENT_SHADER, "debug.frag");
    auto shaderProgram = std::make_shared<ShaderProgram>(vshader, fshader);

    auto camera = PerspectiveCamera(app);

    auto ubo = UniformBuffer("Model", constants::MODEL_BINDING, sizeof(glm::mat4));
    ubo.update(0, glm::value_ptr(glm::mat4(1.0f)), sizeof(glm::mat4));

    shaderProgram->use();
    
    GLuint vbo, ebo;
    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_STORAGE_BIT);
    glCreateBuffers(1, &ebo);
    glNamedBufferStorage(ebo, sizeof(std::uint32_t) * indices.size(), indices.data(), GL_DYNAMIC_STORAGE_BIT);

    GLuint vao;
    glCreateVertexArrays(1, &vao);
    glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(vao, ebo);

    glEnableVertexArrayAttrib(vao, 0);
    glEnableVertexArrayAttrib(vao, 1);
    glEnableVertexArrayAttrib(vao, 2);

    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
    glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, nrm));
    glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, tex));

    glVertexArrayAttribBinding(vao, 0, 0);
    glVertexArrayAttribBinding(vao, 1, 0);
    glVertexArrayAttribBinding(vao, 2, 0);
    
    glBindVertexArray(vao);

    while (!glfwWindowShouldClose(app->getGlCtx()))
    {
        app->updateInternalTimes();
        
        glfwPollEvents();
        camera.tick();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(app->getGlCtx());
    }

    glBindVertexArray(0);

    return 0;
}

//------------------------------------------------------------------------
