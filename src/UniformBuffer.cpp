#include "UniformBuffer.hpp"

//------------------------------------------------------------------------

UniformBuffer::UniformBuffer(const std::string& blockName,
    GLuint binding,
    const std::shared_ptr<ShaderProgram>& shaderProgram,
    const Data& uniformData = Data())
    : m_shaderProgram{shaderProgram}
{
    ShaderProgram::UseGuard useGuard{*m_shaderProgram};
    const GLuint program = m_shaderProgram->getHandle();

    CHECK_GL_ERROR(m_uniformBlockIdx = glGetUniformBlockIndex(program, blockName.c_str()));
    GLint size;
    CHECK_GL_ERROR(glGetActiveUniformBlockiv(program, m_uniformBlockIdx, GL_UNIFORM_BLOCK_DATA_SIZE, &size));

    CHECK_GL_ERROR(glGenBuffers(1, &m_handle));
    CHECK_GL_ERROR(glBindBuffer(GL_UNIFORM_BUFFER, m_handle));
    
    CHECK_GL_ERROR(glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));

    const uint32_t numUniforms = uniformData.size();

    if (numUniforms > 0)
    {
        m_indices.resize(numUniforms);
        m_offset.resize(numUniforms);
        m_size.resize(numUniforms);
        m_type.resize(numUniforms);

        CHECK_GL_ERROR(glGetUniformIndices(program, numUniforms, uniformData.getNames().data(), m_indices.data()));
        CHECK_GL_ERROR(glGetActiveUniformsiv(program, numUniforms, m_indices.data(), GL_UNIFORM_OFFSET, m_offset.data()));
        CHECK_GL_ERROR(glGetActiveUniformsiv(program, numUniforms, m_indices.data(), GL_UNIFORM_SIZE, m_size.data()));
        CHECK_GL_ERROR(glGetActiveUniformsiv(program, numUniforms, m_indices.data(), GL_UNIFORM_TYPE, m_type.data()));
        
        CHECK_GL_ERROR(GLvoid* buffer = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY));
        
        for (auto i = 0u; i < numUniforms; ++i)
        {
            const GLvoid* data = std::get<1>(uniformData.elements[i]);
            std::memcpy((char*)buffer + m_offset[i], data, m_size[i] * Util::getGlTypeSize(m_type[i]));
        }

        CHECK_GL_ERROR(glUnmapBuffer(GL_UNIFORM_BUFFER));
    }

    CHECK_GL_ERROR(glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_handle));

    CHECK_GL_ERROR(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

//------------------------------------------------------------------------

UniformBuffer::~UniformBuffer()
{
    CHECK_GL_ERROR(glDeleteBuffers(1, &m_handle));
}

//------------------------------------------------------------------------
