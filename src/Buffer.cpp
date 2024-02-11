#include "Buffer.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Buffer::Buffer(BufferDescriptor desc)
    : m_usage{desc.usage},
      m_wholeByteSize{desc.byteSize},
      m_managed{desc.managed}
{
    glCreateBuffers(1, &m_name);

    glNamedBufferStorage(m_name, m_wholeByteSize, nullptr, GL_DYNAMIC_STORAGE_BIT | s_access);
    m_ptr = std::bit_cast<uint8_t*>(glMapNamedBufferRange(m_name, 0, m_wholeByteSize, s_access));

    for (BufferUsage::Type target : desc.bindings) {
        bindAs(target);
    }
    for (auto [target, index] : desc.indexedBindings) {
        bindBaseAs(index, target);
    }
}

//------------------------------------------------------------------------

Buffer::~Buffer()
{
    if (not m_managed) freeResources();
}

//------------------------------------------------------------------------

void Buffer::freeResources()
{
    glUnmapNamedBuffer(m_name);
    glDeleteBuffers(1, &m_name);
}

//------------------------------------------------------------------------

void Buffer::bindAs(BufferUsage::Type usage)
{
    glBindBuffer(BufferUsage2GLenum[usage], m_name);
}

//------------------------------------------------------------------------

void Buffer::bindBaseAs(GLuint bindingIndex, BufferUsage::Type usage)
{
    glBindBufferBase(BufferUsage2GLenum[usage], bindingIndex, m_name);
}

//------------------------------------------------------------------------

void Buffer::invalidate(GLintptr offset, GLsizeiptr length)
{
    glInvalidateBufferSubData(m_name, offset, length == 0 ? m_writeOffset : length);
    m_writeOffset = 0;
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
