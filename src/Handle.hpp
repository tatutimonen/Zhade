#pragma once

#include <cstdint>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------
// Adapted from: https://twitter.com/SebAaltonen/status/1534416275828514817.

template<typename T>
class Handle
{
public:
    Handle() = default;
    [[nodiscard]] bool isValid() { return m_generation != 0; }

private:
    Handle(uint32_t index, uint32_t generation) : m_index{index}, m_generation{generation} {}

    uint32_t m_index = 0;
    uint32_t m_generation = 0;

    template<typename U>
    friend class ObjectPool;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
