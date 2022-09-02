#pragma once

#include "Handle.hpp"
#include "Stack.hpp"

#include <memory>

extern "C" {
#include <stdint.h>
}

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

template<typename T>
class ObjectPool
{
public:
    ObjectPool()
        : m_pool{std::make_unique<T[]>(s_size)},
          m_generations{std::make_unique<uint32_t[]>(s_size)}
    {
        for (uint32_t i = s_size - 1; i >= 0; --i)
            m_freeStack.push(i);
    }

    ~ObjectPool() = default;
    
    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;
    ObjectPool(ObjectPool&& other) = default;
    ObjectPool& operator=(ObjectPool&& other) = default;

    [[nodiscard]] constexpr Handle<T> allocate()
    {
        
    }

    constexpr void deallocate()
    {

    }

    static constexpr size_t s_size = 1 << 15;

private:
    std::unique_ptr<T[]> m_pool;
    std::unique_ptr<uint32_t[]> m_generations;
    Stack<uint32_t> m_freeStack;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
