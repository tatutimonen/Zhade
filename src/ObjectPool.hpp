#pragma once

#include "Handle.hpp"
#include "Stack.hpp"

#include <cstdint>
#include <ranges>
#include <vector>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------
// Reference: https://twitter.com/SebAaltonen/status/1535176343847043072.

template<typename T>
class ObjectPool
{
public:
    ObjectPool(size_t size = 1)
        : m_size{size}, m_freeStack{Stack<uint32_t>(size)}
    {
        m_pool.resize(m_size);
        m_generations.resize(m_size);

        for (const size_t& idx : std::views::iota(0u, m_size) | std::views::reverse)
        {
            m_generations[idx] = 0;
            m_freeStack.push(idx);
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
        std::construct_at(std::addressof(m_pool[handle.m_index]), std::forward<Args>(args)...);
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
        const uint32_t getIdx = handle.m_index;
        if (handle.m_generation < m_generations[getIdx])
            return nullptr;

        return std::addressof(m_pool[handle.m_index]);
    }

    //------------------------------------------------------------------------------------
    // TODO: Implement the contiguous_range concept to comply with std::span requirements.
    //------------------------------------------------------------------------------------

private:
    [[nodiscard]] Handle<T> getHandleToNextFree()
    {
        try 
        {
            [[maybe_unused]] const auto& top = m_freeStack.top();
        } 
        catch (const std::out_of_range&)
        {
            resize();
        }
        const uint32_t nextFreeIdx = m_freeStack.top();
        m_freeStack.pop();
        return Handle<T>(nextFreeIdx, ++m_generations[nextFreeIdx]);
    }

    void resize()
    {
        const auto size_prev = m_size;
        m_size *= 2;
        m_pool.resize(m_size);
        m_generations.resize(m_size);
        m_freeStack.m_underlying.reserve(m_size);

        for (const auto& idx : std::views::iota(size_prev, m_size) | std::views::reverse)
        {
            m_generations[idx] = 0;
            m_freeStack.push(idx);
        }
    }

    size_t m_size;
    mutable std::vector<T> m_pool;
    mutable std::vector<uint32_t> m_generations;
    mutable Stack<uint32_t> m_freeStack;

    friend class ResourceManager;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
