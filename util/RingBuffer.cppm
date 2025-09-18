/**
 * @file RingBuffer.cppm
 * @module emdevif.util.ringBuffer
 * @author DuYicheng
 * @date 2025-09-18
 * @brief 环形缓冲区
 */

module;

#include <cstdint>
#include <cstring>

#include <array>
#include <concepts>
#include <span>
#include <type_traits>

export module emdevif.util.ringBuffer;

export namespace emdevif {

template<std::unsigned_integral ValueType, ValueType max_num>
class RingUnsigned
{
public:
    constexpr RingUnsigned() noexcept : value_(0U) {}

    constexpr RingUnsigned(const RingUnsigned&) noexcept = default;
    constexpr RingUnsigned& operator=(const RingUnsigned&) noexcept = default;

    explicit constexpr RingUnsigned(const ValueType init_value) noexcept : value_(init_value % max_num) {}
    constexpr RingUnsigned& operator=(const ValueType init_value) noexcept
    {
        value_ = init_value % max_num;
        return *this;
    }

    constexpr ~RingUnsigned() noexcept = default;

public:
    constexpr RingUnsigned operator++() noexcept
    {
        value_ = (value_ + 1U) % max_num;
        return *this;
    }
    constexpr RingUnsigned operator++(int) noexcept
    {
        const auto temp = value_;
        ++(*this);
        value_ = temp;
        return *this;
    }

    constexpr RingUnsigned operator--() noexcept
    {
        if (value_ == 0U) {
            value_ = max_num;
        }
        else {
            --value_;
        }

        return *this;
    }
    constexpr RingUnsigned operator--(int) noexcept
    {
        const auto temp = value_;
        --(*this);
        value_ = temp;
        return *this;
    }

    constexpr RingUnsigned operator+=(const ValueType rhs) noexcept
    {
        value_ = (value_ + rhs) % max_num;
        return *this;
    }

    constexpr RingUnsigned operator-(const RingUnsigned rhs) const noexcept
    {
        const ValueType lhs_value = value_;
        const ValueType rhs_value = rhs.value_;

        if (lhs_value >= rhs_value) {
            return RingUnsigned(lhs_value - rhs_value);
        }
        else {
            return RingUnsigned(max_num - (rhs_value - lhs_value));
        }
    }

    explicit constexpr operator ValueType() const noexcept
    {
        return value_;
    }

private:
    std::size_t value_;
};

template<typename Type, std::size_t item_size>
    requires(std::copyable<Type> && std::movable<Type>)
class RingBuffer
{
public:
    constexpr RingBuffer() noexcept = default;
    constexpr ~RingBuffer() noexcept = default;

public:
    constexpr void clear() noexcept
    {
        head_ = 0U;
        tail_ = 0U;
    }

    constexpr void push(const Type& item) noexcept
    {
        buffer_[head_++] = item;
    }
    constexpr void push(Type&& item) noexcept
    {
        buffer_[head_++] = std::move(item);
    }

    constexpr void push(const std::span<const Type> items) noexcept
    {
        if (std::is_constant_evaluated()) {
            for (const auto& item : items) {
                push(item);
            }
        }
        else {
            if (item_size - head_ >= items.size()) {
                std::memcpy(&buffer_[head_], items.data(), sizeof(Type) * items.size());
                head_ += items.size();
            }
            else {
                const auto last_remain_slots = item_size - head_;

                std::memcpy(&buffer_[head_], items.data(), sizeof(Type) * last_remain_slots);
                std::memcpy(&buffer_[0], &items[last_remain_slots], sizeof(Type) * (items.size() - last_remain_slots));
                head_ += items.size();
            }
        }
    }

    constexpr void push(std::span<Type>&& items) noexcept
    {
        for (auto& item : items) {
            push(std::move(item));
        }
    }

    constexpr Type pop() noexcept
    {
        const auto& temp = buffer_[tail_++];
        return temp;
    }

    constexpr Type peek() const noexcept
    {
        return buffer_[tail_];
    }

    constexpr void pop(std::span<Type> items) noexcept
    {
        if (std::is_constant_evaluated()) {
            for (auto& item : items) {
                item = pop();
            }
        }
        else {
            if (item_size - tail_ >= items.size()) {
                std::memcpy(items.data(), &buffer_[tail_], sizeof(Type) * items.size());
                tail_ += items.size();
            }
            else {
                const auto last_remain_slots = item_size - tail_;
                std::memcpy(items.data(), &buffer_[tail_], sizeof(Type) * last_remain_slots);
                std::memcpy(&items[last_remain_slots], &buffer_[0], sizeof(Type) * (items.size() - last_remain_slots));
                tail_ += items.size();
            }
        }
    }

    constexpr void peek(std::span<Type> items) const noexcept
    {
        if (std::is_constant_evaluated()) {
            for (auto& item : items) {
                item = peek();
            }
        }
        else {
            if (item_size - tail_ >= items.size()) {
                std::memcpy(items.data(), &buffer_[tail_], sizeof(Type) * items.size());
            }
            else {
                const auto last_remain_slots = item_size - tail_;
                std::memcpy(items.data(), &buffer_[tail_], sizeof(Type) * last_remain_slots);
                std::memcpy(&items[last_remain_slots], &buffer_[0], sizeof(Type) * (items.size() - last_remain_slots));
            }
        }
    }

    [[nodiscard]] constexpr std::size_t usedSlots() const noexcept
    {
        return head_ - tail_;
    }

    [[nodiscard]] constexpr std::size_t usedSizeByte() const noexcept
    {
        return sizeof(Type) * usedSlots();
    }

    [[nodiscard]] constexpr std::size_t remainSlots() const noexcept
    {
        return tail_ - head_;
    }

    [[nodiscard]] constexpr std::size_t remainSizeByte() const noexcept
    {
        return sizeof(Type) * remainSlots();
    }

    [[nodiscard]] consteval std::size_t size() const noexcept
    {
        return item_size;
    }

    [[nodiscard]] consteval std::size_t sizeByte() const noexcept
    {
        return item_size * sizeof(Type);
    }

    constexpr Type& nextSlot() noexcept
    {
        return buffer_[head_];
    }
    constexpr const Type& nextSlot() const noexcept
    {
        return buffer_[head_];
    }

private:
    Type buffer_[item_size];
    RingUnsigned<std::size_t, item_size> head_{0U};
    RingUnsigned<std::size_t, item_size> tail_{0U};
};

}  // namespace emdevif
