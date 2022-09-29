#pragma once

#include "AlignedResource.hpp"
#include "Handle.hpp"
#include "Stack.hpp"

#include <cstdint>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

template<typename T>
class ObjectPool
{
public:
    ObjectPool()
        : m_pool{AlignedResource<T>(s_size)},
          m_generations{AlignedResource<uint32_t>(s_size)},
          m_freeStack{Stack<uint32_t>(s_size)}
    {
        for (uint32_t i = 0; i < s_size; ++i)
        {
            m_generations[i] = 0;
            m_freeStack.push(s_size - 1 - i);
        }
    }

    ~ObjectPool() = default;
    
    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;
    ObjectPool(ObjectPool&& other) = default;
    ObjectPool& operator=(ObjectPool&& other) = default;

    template<typename... Args>
    requires std::is_constructible_v<T, Args...>
    [[nodiscard]] Handle<T> allocate(Args&& ...args) const
    {
        const auto handle = getHandleToNextFree();
        new (&m_pool[handle.m_index]) T(std::forward<Args>(args)...);
        return handle;
    }

    [[nodiscard]] Handle<T> allocate(const T& item) const
    requires std::copyable<T>
    {
        const auto handle = getHandleToNextFree();
        m_pool[handle.m_index] = item;
        return handle;
    }

    [[nodiscard]] Handle<T> allocate(T&& item) const
    requires std::movable<T>
    {
        const auto handle = getHandleToNextFree();
        m_pool[handle.m_index] = std::move(item);
        return handle;
    }

    void deallocate(const Handle<T>& handle) const noexcept
    {
        const uint32_t deallocIdx = handle.m_index;
        m_freeStack.push(deallocIdx);
        ++m_generations[deallocIdx];
    }

    [[nodiscard]] T* get(const Handle<T>& handle) const noexcept
    {
        const auto getIdx = handle.m_index;
        if (handle.m_generation < m_generations[getIdx])
            return nullptr;

        return &m_pool[handle.m_index];
    }

    static constexpr size_t s_size = 10'000;

private:
    [[nodiscard]] Handle<T> getHandleToNextFree() const
    {
        if (m_freeStack.top() == std::nullopt)
            throw std::bad_alloc();

        const uint32_t nextFreeIdx = m_freeStack.top().value();
        m_freeStack.pop();
        return Handle<T>(nextFreeIdx, ++m_generations[nextFreeIdx]);
    }

    mutable AlignedResource<T> m_pool;
    mutable AlignedResource<uint32_t> m_generations;
    mutable Stack<uint32_t> m_freeStack;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
