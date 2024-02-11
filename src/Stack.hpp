#pragma once

#include "common.hpp"

#include <algorithm>
#include <vector>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

template<std::default_initializable T>
class Stack
{
public:
    explicit Stack(size_t capacity = 0)
    {
        if (capacity > 0) resize(capacity);
    }

    [[nodiscard]] size_t size() { return m_size; }

    T& top()
    {
        if (m_size == 0) [[unlikely]] {
            fmt::println("Top of an empty Stack");
        }
        return at(m_size - 1);
    }

    const T& top() const
    {
        if (m_size == 0) [[unlikely]] {
            fmt::println("Top of an empty Stack");
        }
        return at(m_size - 1);
    }

    void pop()
    {
        if (m_size > 0) [[likely]] {
            --m_size;
        }
    }

    void push(const T& item)
    requires std::copyable<T>
    {
        if (m_size == m_underlying.size()) [[unlikely]] {
            resize();
        }
        at(m_size++) = item;
    }

    void push(T&& item)
    requires std::movable<T>
    {
        if (m_size == m_underlying.size()) [[unlikely]] {
            resize();
        }
        at(m_size++) = item;
    }

    template<typename... Args>
    requires std::constructible_from<T, Args...>
    void emplace(Args&& ...args)
    {
        if (m_size == m_underlying.size()) [[unlikely]] {   
            resize();
        }
        std::construct_at(&this[m_size++], std::forward<Args>(args)...);
    }

    void resize(size_t size)
    {
        m_underlying.resize(size);
        m_size = std::min(m_size, m_underlying.size());
    }

    [[nodiscard]] T& at(size_t pos)
    {
        return m_underlying[pos];
    }

    [[nodiscard]] const T& at(size_t pos) const
    {
        return m_underlying[pos];
    }

    [[nodiscard]] T& operator[](size_t pos)
    {
        return m_underlying[pos];
    }

private:
    void resize()
    {
        m_underlying.resize(std::max(implicit_cast<size_t>(1), m_size) * DYNAMIC_STORAGE_GROWTH_FACTOR);
    }

    std::vector<T> m_underlying;
    size_t m_size = 0;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
