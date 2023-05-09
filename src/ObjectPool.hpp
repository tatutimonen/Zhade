#pragma once

#include "Handle.hpp"
#include "Stack.hpp"
#include "constants.hpp"

#include <algorithm>
#include <cstdint>
#include <ranges>
#include <vector>
#include <iostream>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------
// Inspired by: https://twitter.com/SebAaltonen/status/1535176343847043072.

template<std::default_initializable T>
class ObjectPool<T>
{
public:
    ObjectPool(size_t size = constants::OBJECT_POOL_INIT_SIZE)
        : m_size{size},
          m_freeList{Stack<uint32_t>(size)}
    {
        m_pool.resize(m_size);
        m_generations.resize(m_size);

        for (size_t idx : std::views::iota(0u, m_size) | std::views::reverse)
        {
            m_generations[idx] = 0;
            m_freeList.push(idx);
        }
    }

    ~ObjectPool()
    {
        if constexpr (requires { T::freeResources(); })
        {
            for (const auto& item : m_pool)
                item.freeResources();
        }
    }

    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;
    ObjectPool(ObjectPool&& other) = default;
    ObjectPool& operator=(ObjectPool&& other) = default;

    [[nodiscard]] size_t size() const noexcept { return m_size; }

    template<typename... Args>
    requires std::constructible_from<T, Args...>
    [[nodiscard]] Handle<T> allocate(Args&& ...args)
    {
        const auto handle = getHandleToNextFree();
        std::construct_at(&m_pool.at(handle.m_index), std::forward<Args>(args)...);
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
        if constexpr (requires { T::freeResources(); })
        {
            if (const T* ptr = get(handle); ptr != nullptr) [[likely]]
                ptr->freeResources();
        }
        const uint32_t deallocIdx = handle.m_index;
        m_freeList.push(deallocIdx);
        ++m_generations[deallocIdx];
    }

    [[nodiscard]] T* get(const Handle<T>& handle) const noexcept
    {
        const uint32_t getIdx = handle.m_index;
        if (handle.m_generation < m_generations.at(getIdx)) return nullptr;
        return &m_pool.at(handle.m_index);
    }

private:
    [[nodiscard]] Handle<T> getHandleToNextFree()
    {
        if (m_freeList.isSaturated()) [[unlikely]] resize();
        const uint32_t nextFreeIdx = m_freeList.top();
        m_freeList.pop();
        return Handle<T>(nextFreeIdx, ++m_generations[nextFreeIdx]);
    }

    void resize()
    {
        const size_t size_prev = m_size;
        m_size = std::max(1ull, m_size) * constants::DYNAMIC_STORAGE_GROWTH_FACTOR;

        m_pool.resize(m_size);
        m_generations.resize(m_size);
        m_freeList.resize(m_size);

        for (size_t idx : std::views::iota(size_prev, m_size) | std::views::reverse)
        {
            m_generations[idx] = 0;
            m_freeList.push(idx);
        }
    }

    size_t m_size;
    mutable std::vector<T> m_pool;
    mutable std::vector<uint32_t> m_generations;
    mutable Stack<uint32_t> m_freeList;

    friend class ResourceManager;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
