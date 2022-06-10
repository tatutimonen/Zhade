#include "VertexFormat.hpp"

//------------------------------------------------------------------------

VertexFormat::VertexFormat(std::initializer_list<Element> elems)
    : m_elements{elems}
{
    computeOffsetsAndStride();
}

//------------------------------------------------------------------------

void VertexFormat::computeOffsetsAndStride() noexcept
{
    auto runningOffset = 0ll;
    for (auto& elem : m_elements)
    {
        elem.offset = runningOffset;
        runningOffset += elem.sizeBytes;
    }
    m_stride = runningOffset;
}

//------------------------------------------------------------------------
