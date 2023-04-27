#pragma once

#include <memory>
#include <stdexcept>
#include <vector>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------
// Because std::stack is horrendously slow and std::pmr::deque with
// a std::pmr::monotonic_buffer_resource is not always applicable as a remedy.

template<std::default_initializable T>
class Stack
{
public:
    Stack(size_t capacity = 0) { if (capacity > 0) [[likely]] resize(capacity); }
    ~Stack() = default;

    Stack(const Stack&) = default;
    Stack& operator=(const Stack&) = default;
    Stack(Stack&&) = default;
    Stack& operator=(Stack&&) = default;

    [[nodiscard]] size_t size() const noexcept { return m_size; }
    [[nodiscard]] bool isSaturated() const noexcept { return m_size == m_underlying.size(); }

    T& top()
    {
        if (m_size == 0) [[unlikely]]
            throw std::out_of_range("Top of an empty Stack");
        return at(m_size - 1);
    }

    const T& top() const
    {
        if (m_size == 0) [[unlikely]]
            throw std::out_of_range("Top of an empty Stack");
        return at(m_size - 1);
    }

    void pop()
    {
        if (m_size == 0) [[unlikely]]
            throw std::out_of_range("Pop on an empty Stack");
        --m_size;
    }

    void push(const T& item) noexcept
    requires std::copyable<T>
    {
        if (isSaturated()) [[unlikely]] resize(m_size * s_growthFactor);
        at(m_size++) = item;
    }

    void push(T&& item) noexcept
    requires std::movable<T>
    {
        if (isSaturated()) [[unlikely]] resize(m_size * s_growthFactor);
        at(m_size++) = item;
    }

    template<typename... Args>
    requires std::constructible_from<T, Args...>
    void emplace(Args&& ...args) noexcept
    {
        if (isSaturated()) [[unlikely]] resize(m_size * s_growthFactor);
        std::construct_at(&at(m_size++), std::forward<Args>(args)...);
    }

    void resize(size_t capacity) noexcept
    {
        m_underlying.resize(capacity);
    }

    [[nodiscard]] T& at(size_t pos)
    {
        return m_underlying.at(pos);
    }

    [[nodiscard]] const T& at(size_t pos) const
    {
        return m_underlying.at(pos);
    }

    [[nodiscard]] T& operator[](size_t pos)
    {
        return m_underlying.at(pos);
    }

    static constexpr uint32_t s_growthFactor = 2;

private:
    std::vector<T> m_underlying;
    size_t m_size = 0;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
