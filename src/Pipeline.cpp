#include "Pipeline.hpp"

#include <fstream>
#include <sstream>
#include <string_view>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Pipeline::Pipeline(PipelineDescriptor desc)
    : m_headers{desc.headers.cbegin(), desc.headers.cend()},
      m_managed{desc.managed}
{
    glCreateProgramPipelines(1, &m_name);
    setupHeaders();
    setupStageProgram(PipelineStage::VERTEX, desc.vertPath);
    setupStageProgram(PipelineStage::FRAGMENT, desc.fragPath);
    setupStageProgram(PipelineStage::GEOMETRY, desc.geomPath);
    setupStageProgram(PipelineStage::COMPUTE, desc.compPath);
    validate();
}

//------------------------------------------------------------------------

Pipeline::~Pipeline()
{
    if (not m_managed) freeResources();
}

//------------------------------------------------------------------------

void Pipeline::freeResources()
{
    glDeleteProgramPipelines(1, &m_name);
    for (GLuint program : m_stages) {
        if (glIsProgram(program)) {
            glDeleteProgram(program);
        }
    }
    for (std::string_view header : m_headers) {
        glDeleteNamedStringARB(header.size(), header.data());
    }
}

//------------------------------------------------------------------------

std::string Pipeline::readFileContents(const fs::path& path)
{
    std::ifstream file{path};
    if (file.bad()) {
        fmt::println("Error reading shader from {}", path.string());
        return "";
    }
    std::ostringstream osstream;
    osstream << file.rdbuf();
    return osstream.str();
}

//------------------------------------------------------------------------

GLuint Pipeline::createShaderProgramInclude(PipelineStage::Type stage, const fs::path& shaderPath)
{
    const GLuint shader = glCreateShader(PipelineStage2GLShader[stage]);
    const std::string shaderSource = readFileContents(shaderPath);
    const char* shaderSourceRaw = shaderSource.c_str();
    glShaderSource(shader, 1, &shaderSourceRaw, nullptr);
    static const GLchar* virtualIncludePaths[] = { "/" };
    glCompileShaderIncludeARB(shader, sizeof(virtualIncludePaths) / sizeof(GLchar*), virtualIncludePaths, nullptr);
    
    GLint compileStatus = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE) {
        GLchar infoLog[LOCAL_CHAR_BUF_SIZE];
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        fmt::println("Error compiling shader {}: {}", shaderPath.string(), infoLog);
        return 0;
    }

    const GLuint program = glCreateProgram();
    glProgramParameteri(program, GL_PROGRAM_SEPARABLE, GL_TRUE);
    glAttachShader(program, shader);
    glLinkProgram(program);
    glDetachShader(program, shader);
    glDeleteShader(shader);
    
    GLint linkStatus = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE) {
        GLchar infoLog[LOCAL_CHAR_BUF_SIZE];
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
        fmt::println("Error linking shader {}: {}", shaderPath.string(), infoLog);
        return 0;
    }

    return program;
}

//------------------------------------------------------------------------

void Pipeline::setupHeaders()
{
    for (std::string_view header : m_headers) {
        const fs::path path = SOURCE_PATH / fs::path{header}.filename();
        const std::string contents = readFileContents(path);
        glNamedStringARB(GL_SHADER_INCLUDE_ARB, header.size(), header.data(), contents.size(), contents.data());
    }
}

//------------------------------------------------------------------------

void Pipeline::setupStageProgram(PipelineStage::Type stage, const fs::path& shaderPath)
{
    if (shaderPath.empty()) return;
    m_stages[stage] = createShaderProgramInclude(stage, shaderPath);
    glUseProgramStages(m_name, PipelineStage2GLShaderBit[stage], m_stages[stage]);
}

//------------------------------------------------------------------------

void Pipeline::validate()
{
    glValidateProgramPipeline(m_name);

    GLint status = GL_FALSE;
    glGetProgramPipelineiv(m_name, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE) {
        GLchar infoLog[LOCAL_CHAR_BUF_SIZE];
        glGetProgramPipelineInfoLog(m_name, sizeof(infoLog), nullptr, infoLog);
        fmt::println("Error validating pipeline with ID {}: {}", m_name, infoLog);
    }
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
