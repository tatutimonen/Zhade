#pragma once

#include "Handle.hpp"
#include "Stack.hpp"

#include <cstdint>
#include <vector>
#include <iostream>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------
// Reference: https://twitter.com/SebAaltonen/status/1535176343847043072.

template<typename T>
class ObjectPool
{
public:
    ObjectPool(size_t size = 1'000)
        : m_size{size}, m_freeStack{Stack<uint32_t>(size)}
    {
        m_pool.resize(m_size);
        m_generations.resize(m_size);

        for (uint32_t i = 0; i < m_size; ++i)
        {
            m_generations[i] = 0;
            m_freeStack.push(i);
        }
    }

    ~ObjectPool() = default;
    
    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;
    ObjectPool(ObjectPool&& other) = default;
    ObjectPool& operator=(ObjectPool&& other) = default;

    [[nodiscard]] size_t getSize() const noexcept { return m_size; }

    template<typename... Args>
    requires std::constructible_from<T, Args...>
    [[nodiscard]] Handle<T> allocate(Args&& ...args)
    {
        const auto handle = getHandleToNextFree();
        new (&m_pool[handle.m_index]) T(std::forward<Args>(args)...);
        return handle;
    }

    [[nodiscard]] Handle<T> allocate(const T& item)
    requires std::copyable<T>
    {
        const auto handle = getHandleToNextFree();
        m_pool[handle.m_index] = item;
        return handle;
    }

    [[nodiscard]] Handle<T> allocate(T&& item)
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

private:
    [[nodiscard]] Handle<T> getHandleToNextFree()
    {
        if (m_freeStack.top() == std::nullopt) [[unlikely]]
            resize();

        const uint32_t nextFreeIdx = m_freeStack.top().value();
        m_freeStack.pop();
        return Handle<T>(nextFreeIdx, ++m_generations[nextFreeIdx]);
    }

    void resize()
    {
        const auto size_prev = m_size;
        m_size *= 2;
        m_pool.resize(m_size);
        m_generations.resize(m_size);
        m_freeStack.resize();

        for (uint32_t i = size_prev; i < m_size; ++i)
        {
            m_generations[i] = 0;
            m_freeStack.push(i);
        }
    }

    size_t m_size;
    mutable std::vector<T> m_pool;
    mutable std::vector<uint32_t> m_generations;
    mutable Stack<uint32_t> m_freeStack;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
