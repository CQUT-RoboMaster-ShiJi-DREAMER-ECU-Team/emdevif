/**
 * @file inplace_dynamic_array.hpp
 * @brief 固定容量的原地动态数组
 *
 * 参考 C++26 std::inplace_vector 接口，提供固定容量、原地构造的序列容器。
 * 所有可能超出容量的操作均返回错误码，不抛异常。
 */

#pragma once
#ifndef EMDEVIF_CORE_DATA_CONTAINER_INPLACE_DYNAMIC_ARRAY_HPP
#define EMDEVIF_CORE_DATA_CONTAINER_INPLACE_DYNAMIC_ARRAY_HPP

#include <cstddef>

#include <algorithm>
#include <compare>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

#include "emdevif/core/fatal_handler.h"

#include "emdevif/core/error_handler.hpp"

namespace emdevif {

/**
 * 固定容量的原地动态数组
 * @tparam T 元素类型
 * @tparam N 最大容量，必须大于 0
 */
template<typename T, std::size_t N>
class InplaceDynamicArray
{
public:
    static_assert(N > 0, "InplaceDynamicArray capacity must be greater than zero");

    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    alignas(T) unsigned char store_[sizeof(T) * N];
    size_type size_{};

    [[nodiscard]] constexpr pointer ptrAt_(size_type index) noexcept
    {
        return static_cast<pointer>(static_cast<void*>(store_ + index * sizeof(T)));
    }

    [[nodiscard]] constexpr const_pointer ptrAt_(size_type index) const noexcept
    {
        return static_cast<const_pointer>(static_cast<const void*>(store_ + index * sizeof(T)));
    }

public:
    // ---------- 容量 ----------

    /**
     * 获取最大容量
     * @return 最大容量
     */
    [[nodiscard]] static constexpr size_type capacity() noexcept
    {
        return N;
    }

    /**
     * 获取最大可容纳元素数
     * @return 最大元素数
     */
    [[nodiscard]] static constexpr size_type maxSize() noexcept
    {
        return N;
    }

    /**
     * 请求预留容量
     * @param new_cap 请求的容量
     * @retval ErrorCode::Success 请求成功
     * @retval ErrorCode::InvalidArgument 请求容量超过固定上限
     */
    [[nodiscard]] constexpr ErrorCode reserve(size_type new_cap) noexcept
    {
        if (new_cap > N) {
            return ErrorCode::InvalidArgument;
        }
        return ErrorCode::Success;
    }

    /**
     * 收缩到合适大小（原地数组无堆分配，无实际效果）
     */
    constexpr void shrinkToFit() noexcept {}

    // ---------- 大小 ----------

    /**
     * 获取当前元素数量
     * @return 当前元素数量
     */
    [[nodiscard]] constexpr size_type size() const noexcept
    {
        return size_;
    }

    /**
     * 判断是否为空
     * @return 是否为空
     */
    [[nodiscard]] constexpr bool isEmpty() const noexcept
    {
        return size_ == 0;
    }

    /**
     * 判断是否已满
     * @return 是否已满
     */
    [[nodiscard]] constexpr bool isFull() const noexcept
    {
        return size_ == N;
    }

    // ---------- 元素访问 ----------

    /**
     * 下标访问（无边界检查）
     * @param index 索引
     * @return 对应元素引用
     */
    [[nodiscard]] constexpr reference operator[](size_type index) noexcept
    {
        EMDEVIF_ASSERT(index < size_, "[InplaceDynamicArray]: index out of range");
        return *ptrAt_(index);
    }
    /**
     * @overload
     */
    [[nodiscard]] constexpr const_reference operator[](size_type index) const noexcept
    {
        EMDEVIF_ASSERT(index < size_, "[InplaceDynamicArray]: index out of range");
        return *ptrAt_(index);
    }

    /**
     * 带边界检查的访问
     * @param index 索引
     * @return 对应元素指针，越界时返回 nullptr
     */
    [[nodiscard]] constexpr pointer at(size_type index) noexcept
    {
        if (index >= size_) {
            return nullptr;
        }
        return ptrAt_(index);
    }
    /**
     * @overload
     */
    [[nodiscard]] constexpr const_pointer at(size_type index) const noexcept
    {
        if (index >= size_) {
            return nullptr;
        }
        return ptrAt_(index);
    }

    /**
     * 访问首元素
     * @return 首元素引用
     */
    [[nodiscard]] constexpr reference front() noexcept
    {
        EMDEVIF_ASSERT(!isEmpty(), "[InplaceDynamicArray]: front() on empty array");
        return *ptrAt_(0);
    }
    /**
     * @overload
     */
    [[nodiscard]] constexpr const_reference front() const noexcept
    {
        EMDEVIF_ASSERT(!isEmpty(), "[InplaceDynamicArray]: front() on empty array");
        return *ptrAt_(0);
    }

    /**
     * 访问末元素
     * @return 末元素引用
     */
    [[nodiscard]] constexpr reference back() noexcept
    {
        EMDEVIF_ASSERT(!isEmpty(), "[InplaceDynamicArray]: back() on empty array");
        return *ptrAt_(size_ - 1);
    }
    /**
     * @overload
     */
    [[nodiscard]] constexpr const_reference back() const noexcept
    {
        EMDEVIF_ASSERT(!isEmpty(), "[InplaceDynamicArray]: back() on empty array");
        return *ptrAt_(size_ - 1);
    }

    /**
     * 获取底层数据指针
     * @return 数据指针
     */
    [[nodiscard]] constexpr pointer data() noexcept
    {
        return ptrAt_(0);
    }
    /**
     * @overload
     */
    [[nodiscard]] constexpr const_pointer data() const noexcept
    {
        return ptrAt_(0);
    }

    // ---------- 迭代器 ----------

    [[nodiscard]] constexpr iterator begin() noexcept
    {
        return ptrAt_(0);
    }

    [[nodiscard]] constexpr const_iterator begin() const noexcept
    {
        return ptrAt_(0);
    }

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept
    {
        return ptrAt_(0);
    }

    [[nodiscard]] constexpr iterator end() noexcept
    {
        return ptrAt_(size_);
    }

    [[nodiscard]] constexpr const_iterator end() const noexcept
    {
        return ptrAt_(size_);
    }

    [[nodiscard]] constexpr const_iterator cend() const noexcept
    {
        return ptrAt_(size_);
    }

    [[nodiscard]] constexpr reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(cend());
    }

    [[nodiscard]] constexpr reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }

    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(cbegin());
    }

    // ---------- 构造 / 析构 ----------

    constexpr InplaceDynamicArray() noexcept = default;

    constexpr InplaceDynamicArray(const InplaceDynamicArray& other) noexcept(std::is_nothrow_copy_constructible_v<T>)
        : size_(other.size_)
    {
        for (size_type i = 0; i < size_; ++i) {
            std::construct_at(ptrAt_(i), other[i]);
        }
    }

    constexpr InplaceDynamicArray(InplaceDynamicArray&& other) noexcept(std::is_nothrow_move_constructible_v<T>)
        : size_(other.size_)
    {
        for (size_type i = 0; i < size_; ++i) {
            std::construct_at(ptrAt_(i), std::move(other[i]));
        }
    }

    constexpr InplaceDynamicArray(std::initializer_list<T> ilist) noexcept(std::is_nothrow_copy_constructible_v<T>)
        : size_(ilist.size())
    {
        EMDEVIF_ASSERT(ilist.size() <= N, "[InplaceDynamicArray]: initializer list too large");
        size_type i = 0;
        for (const auto& value : ilist) {
            std::construct_at(ptrAt_(i), value);
            ++i;
        }
    }

    ~InplaceDynamicArray()
        requires(std::is_trivially_destructible_v<T>)
    = default;

    constexpr ~InplaceDynamicArray()
        requires(!std::is_trivially_destructible_v<T>)
    {
        std::destroy(begin(), end());
    }

    // ---------- 赋值 ----------

    constexpr InplaceDynamicArray& operator=(const InplaceDynamicArray& other) noexcept(
        std::is_nothrow_copy_assignable_v<T> && std::is_nothrow_copy_constructible_v<T>)
    {
        if (this == &other) {
            return *this;
        }
        const size_type common = (std::min)(size_, other.size_);

        for (size_type i = 0; i < common; ++i) {
            (*this)[i] = other[i];
        }

        if (size_ > other.size_) {
            std::destroy(ptrAt_(common), ptrAt_(size_));
        }
        else if (other.size_ > size_) {
            for (size_type i = common; i < other.size_; ++i) {
                std::construct_at(ptrAt_(i), other[i]);
            }
        }

        size_ = other.size_;
        return *this;
    }

    constexpr InplaceDynamicArray& operator=(InplaceDynamicArray&& other) noexcept(
        std::is_nothrow_move_assignable_v<T> && std::is_nothrow_move_constructible_v<T>)
    {
        if (this == &other) {
            return *this;
        }
        const size_type common = (std::min)(size_, other.size_);

        for (size_type i = 0; i < common; ++i) {
            (*this)[i] = std::move(other[i]);
        }

        if (size_ > other.size_) {
            std::destroy(ptrAt_(common), ptrAt_(size_));
        }
        else if (other.size_ > size_) {
            for (size_type i = common; i < other.size_; ++i) {
                std::construct_at(ptrAt_(i), std::move(other[i]));
            }
        }

        size_ = other.size_;
        return *this;
    }

    constexpr InplaceDynamicArray& operator=(std::initializer_list<T> ilist) noexcept(
        std::is_nothrow_copy_assignable_v<T> && std::is_nothrow_copy_constructible_v<T>)
    {
        EMDEVIF_ASSERT(ilist.size() <= N, "[InplaceDynamicArray]: initializer list too large");
        const auto result = assign(ilist);
        EMDEVIF_UNUSED(result);
        return *this;
    }

    // ---------- assign ----------

    /**
     * 用 count 个 value 填充
     * @param count 元素数量
     * @param value 填充值
     * @retval ErrorCode::Success 成功
     * @retval ErrorCode::InvalidArgument count 超过容量
     */
    constexpr ErrorCode assign(size_type count,
                               const value_type& value) noexcept(std::is_nothrow_copy_assignable_v<T> &&
                                                                 std::is_nothrow_copy_constructible_v<T>)
    {
        if (count > N) {
            return ErrorCode::InvalidArgument;
        }

        if (count < size_) {
            std::fill_n(ptrAt_(0), count, value);
            std::destroy(ptrAt_(count), ptrAt_(size_));
        }
        else {
            std::fill_n(ptrAt_(0), size_, value);
            for (size_type i = size_; i < count; ++i) {
                std::construct_at(ptrAt_(i), value);
            }
        }

        size_ = count;
        return ErrorCode::Success;
    }

    /**
     * 用迭代器范围赋值
     * @tparam InputIt 输入迭代器类型
     * @param first 范围起始
     * @param last 范围结束
     * @retval ErrorCode::Success 成功
     * @retval ErrorCode::InvalidArgument 范围大小超过容量
     */
    template<typename InputIt>
        requires(!std::is_integral_v<InputIt>)
    constexpr ErrorCode assign(InputIt first,
                               InputIt last) noexcept(std::is_nothrow_assignable_v<reference, decltype(*first)> &&
                                                      std::is_nothrow_constructible_v<value_type, decltype(*first)>)
    {
        const size_type count = static_cast<size_type>(std::distance(first, last));
        if (count > N) {
            return ErrorCode::InvalidArgument;
        }

        if (count < size_) {
            std::copy(first, last, ptrAt_(0));
            std::destroy(ptrAt_(count), ptrAt_(size_));
        }
        else {
            auto mid = std::next(first, static_cast<difference_type>(size_));
            std::copy(first, mid, ptrAt_(0));
            for (auto it = mid; it != last; ++it) {
                std::construct_at(ptrAt_(size_ + static_cast<size_type>(std::distance(mid, it))), *it);
            }
        }

        size_ = count;
        return ErrorCode::Success;
    }

    /**
     * 用初始化列表赋值
     * @param ilist 初始化列表
     * @retval ErrorCode::Success 成功
     * @retval ErrorCode::InvalidArgument 列表大小超过容量
     */
    constexpr ErrorCode assign(std::initializer_list<T> ilist) noexcept(std::is_nothrow_copy_assignable_v<T> &&
                                                                        std::is_nothrow_copy_constructible_v<T>)
    {
        return assign(ilist.begin(), ilist.end());
    }

    /**
     * 用范围赋值
     * @tparam Range 范围类型
     * @param range 范围
     * @retval ErrorCode::Success 成功
     * @retval ErrorCode::InvalidArgument 范围大小超过容量
     */
    template<typename Range>
    constexpr ErrorCode assignRange(Range&& range) noexcept(noexcept(assign(std::begin(range), std::end(range))))
    {
        return assign(std::begin(range), std::end(range));
    }

    // ---------- resize ----------

    /**
     * 调整大小（值初始化新增元素）
     * @param new_size 新大小
     * @retval ErrorCode::Success 成功
     * @retval ErrorCode::InvalidArgument 新大小超过容量
     */
    constexpr ErrorCode resize(size_type new_size) noexcept(std::is_nothrow_constructible_v<T>)
    {
        if (new_size > N) {
            return ErrorCode::InvalidArgument;
        }

        if (new_size > size_) {
            for (size_type i = size_; i < new_size; ++i) {
                std::construct_at(ptrAt_(i));
            }
        }
        else if (new_size < size_) {
            std::destroy(ptrAt_(new_size), ptrAt_(size_));
        }

        size_ = new_size;
        return ErrorCode::Success;
    }

    /**
     * 调整大小（用指定值填充新增元素）
     * @param new_size 新大小
     * @param value 填充值
     * @retval ErrorCode::Success 成功
     * @retval ErrorCode::InvalidArgument 新大小超过容量
     */
    constexpr ErrorCode resize(size_type new_size,
                               const value_type& value) noexcept(std::is_nothrow_copy_constructible_v<T>)
    {
        if (new_size > N) {
            return ErrorCode::InvalidArgument;
        }

        if (new_size > size_) {
            for (size_type i = size_; i < new_size; ++i) {
                std::construct_at(ptrAt_(i), value);
            }
        }
        else if (new_size < size_) {
            std::destroy(ptrAt_(new_size), ptrAt_(size_));
        }

        size_ = new_size;
        return ErrorCode::Success;
    }

    // ---------- clear ----------

    /**
     * 清空所有元素
     */
    constexpr void clear() noexcept
    {
        std::destroy(begin(), end());
        size_ = 0;
    }

    // ---------- push_back ----------

    /**
     * 尾部追加元素
     * @param value 待追加元素
     * @retval ErrorCode::Success 成功
     * @retval ErrorCode::Full 已满
     */
    constexpr ErrorCode pushBack(const value_type& value) noexcept(std::is_nothrow_copy_constructible_v<T>)
    {
        if (isFull()) {
            return ErrorCode::Full;
        }
        std::construct_at(ptrAt_(size_), value);
        ++size_;
        return ErrorCode::Success;
    }

    /**
     * @overload
     */
    constexpr ErrorCode pushBack(value_type&& value) noexcept(std::is_nothrow_move_constructible_v<T>)
    {
        if (isFull()) {
            return ErrorCode::Full;
        }
        std::construct_at(ptrAt_(size_), std::move(value));
        ++size_;
        return ErrorCode::Success;
    }

    /**
     * 无检查地尾部追加元素
     * @param value 待追加元素
     * @return 追加元素地址
     */
    constexpr pointer uncheckedPushBack(const value_type& value) noexcept(std::is_nothrow_copy_constructible_v<T>)
    {
        std::construct_at(ptrAt_(size_), value);
        ++size_;
        return ptrAt_(size_ - 1);
    }

    /**
     * @overload
     */
    constexpr pointer uncheckedPushBack(value_type&& value) noexcept(std::is_nothrow_move_constructible_v<T>)
    {
        std::construct_at(ptrAt_(size_), std::move(value));
        ++size_;
        return ptrAt_(size_ - 1);
    }

    /**
     * 尝试尾部追加元素
     * @param value 待追加元素
     * @return 追加元素地址，失败返回 nullptr
     */
    [[nodiscard]] constexpr pointer tryPushBack(const value_type& value) noexcept(
        std::is_nothrow_copy_constructible_v<T>)
    {
        if (isFull()) {
            return nullptr;
        }
        return uncheckedPushBack(value);
    }

    /**
     * @overload
     */
    [[nodiscard]] constexpr pointer tryPushBack(value_type&& value) noexcept(std::is_nothrow_move_constructible_v<T>)
    {
        if (isFull()) {
            return nullptr;
        }
        return uncheckedPushBack(std::move(value));
    }

    // ---------- emplace_back ----------

    /**
     * 尾部原位构造
     * @tparam Args 构造参数类型
     * @param args 构造参数
     * @retval ErrorCode::Success 成功
     * @retval ErrorCode::Full 已满
     */
    template<typename... Args>
    constexpr ErrorCode emplaceBack(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
    {
        if (isFull()) {
            return ErrorCode::Full;
        }
        std::construct_at(ptrAt_(size_), std::forward<Args>(args)...);
        ++size_;
        return ErrorCode::Success;
    }

    /**
     * 无检查地尾部原位构造
     * @tparam Args 构造参数类型
     * @param args 构造参数
     * @return 构造元素地址
     */
    template<typename... Args>
    constexpr pointer uncheckedEmplaceBack(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
    {
        std::construct_at(ptrAt_(size_), std::forward<Args>(args)...);
        ++size_;
        return ptrAt_(size_ - 1);
    }

    /**
     * 尝试尾部原位构造
     * @tparam Args 构造参数类型
     * @param args 构造参数
     * @return 构造元素地址，失败返回 nullptr
     */
    template<typename... Args>
    [[nodiscard]] constexpr pointer tryEmplaceBack(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
    {
        if (isFull()) {
            return nullptr;
        }
        return uncheckedEmplaceBack(std::forward<Args>(args)...);
    }

    // ---------- pop_back ----------

    /**
     * 移除末尾元素
     */
    constexpr void popBack() noexcept
    {
        if (!isEmpty()) {
            --size_;
            std::destroy_at(ptrAt_(size_));
        }
    }

    // ---------- emplace ----------

    /**
     * 在指定位置原位构造
     * @tparam Args 构造参数类型
     * @param pos 插入位置
     * @param args 构造参数
     * @return first 为插入位置迭代器，second 为错误码
     */
    template<typename... Args>
    constexpr std::pair<iterator, ErrorCode> emplace(const_iterator pos, Args&&... args) noexcept(
        std::is_nothrow_move_constructible_v<T> && std::is_nothrow_constructible_v<T, Args...>)
    {
        const auto index = static_cast<size_type>(std::distance(cbegin(), pos));
        EMDEVIF_ASSERT(cbegin() <= pos && pos <= cend(), "[InplaceDynamicArray]: iterator out of range");

        if (isFull()) {
            return {nullptr, ErrorCode::Full};
        }

        if (index == size_) {
            std::construct_at(ptrAt_(size_), std::forward<Args>(args)...);
            ++size_;
            return {ptrAt_(index), ErrorCode::Success};
        }

        std::construct_at(ptrAt_(size_), std::move(*ptrAt_(size_ - 1)));
        std::move_backward(ptrAt_(index), ptrAt_(size_ - 1), ptrAt_(size_));
        std::destroy_at(ptrAt_(index));
        std::construct_at(ptrAt_(index), std::forward<Args>(args)...);
        ++size_;
        return {ptrAt_(index), ErrorCode::Success};
    }

    // ---------- insert ----------

    /**
     * 在指定位置插入单个元素
     * @param pos 插入位置
     * @param value 待插入元素
     * @return first 为插入位置迭代器，second 为错误码
     */
    constexpr std::pair<iterator, ErrorCode> insert(const_iterator pos, const value_type& value) noexcept(
        std::is_nothrow_move_constructible_v<T> && std::is_nothrow_copy_constructible_v<T>)
    {
        return emplace(pos, value);
    }

    /**
     * @overload
     */
    constexpr std::pair<iterator, ErrorCode> insert(const_iterator pos, value_type&& value) noexcept(
        std::is_nothrow_move_constructible_v<T>)
    {
        return emplace(pos, std::move(value));
    }

    /**
     * 在指定位置插入 count 个 value
     * @param pos 插入位置
     * @param count 数量
     * @param value 值
     * @return first 为插入起始迭代器，second 为错误码
     */
    constexpr std::pair<iterator, ErrorCode> insert(
        const_iterator pos,
        size_type count,
        const value_type& value) noexcept(std::is_nothrow_move_constructible_v<T> &&
                                          std::is_nothrow_copy_assignable_v<T> &&
                                          std::is_nothrow_copy_constructible_v<T>)
    {
        const auto index = static_cast<size_type>(std::distance(cbegin(), pos));
        EMDEVIF_ASSERT(cbegin() <= pos && pos <= cend(), "[InplaceDynamicArray]: iterator out of range");

        if (count == 0) {
            return {ptrAt_(index), ErrorCode::Success};
        }
        if (size_ + count > N) {
            return {nullptr, ErrorCode::Full};
        }

        const size_type tail = size_ - index;
        const size_type uninit = (std::min)(tail, count);

        for (size_type i = 0; i < uninit; ++i) {
            std::construct_at(ptrAt_(size_ + count - 1 - i), std::move(*ptrAt_(size_ - 1 - i)));
        }
        if (tail > uninit) {
            std::move_backward(ptrAt_(index), ptrAt_(size_ - uninit), ptrAt_(size_ + count - uninit));
        }

        const size_type fill_existing = (std::min)(count, size_ - index);
        if (fill_existing > 0) {
            std::fill_n(ptrAt_(index), fill_existing, value);
        }
        for (size_type i = fill_existing; i < count; ++i) {
            std::construct_at(ptrAt_(size_ + i - fill_existing), value);
        }

        size_ += count;
        return {ptrAt_(index), ErrorCode::Success};
    }

    /**
     * 在指定位置插入迭代器范围
     * @tparam InputIt 输入迭代器类型
     * @param pos 插入位置
     * @param first 范围起始
     * @param last 范围结束
     * @return first 为插入起始迭代器，second 为错误码
     */
    template<typename InputIt>
        requires(!std::is_integral_v<InputIt>)
    constexpr std::pair<iterator, ErrorCode> insert(const_iterator pos, InputIt first, InputIt last) noexcept(
        std::is_nothrow_move_constructible_v<T> && std::is_nothrow_assignable_v<reference, decltype(*first)> &&
        std::is_nothrow_constructible_v<value_type, decltype(*first)>)
    {
        const auto index = static_cast<size_type>(std::distance(cbegin(), pos));
        EMDEVIF_ASSERT(cbegin() <= pos && pos <= cend(), "[InplaceDynamicArray]: iterator out of range");

        const auto count = static_cast<size_type>(std::distance(first, last));
        if (count == 0) {
            return {ptrAt_(index), ErrorCode::Success};
        }
        if (size_ + count > N) {
            return {nullptr, ErrorCode::Full};
        }

        const size_type tail = size_ - index;
        const size_type uninit = (std::min)(tail, count);

        for (size_type i = 0; i < uninit; ++i) {
            std::construct_at(ptrAt_(size_ + count - 1 - i), std::move(*ptrAt_(size_ - 1 - i)));
        }
        if (tail > uninit) {
            std::move_backward(ptrAt_(index), ptrAt_(size_ - uninit), ptrAt_(size_ + count - uninit));
        }

        const size_type fill_existing = (std::min)(count, size_ - index);
        if (fill_existing > 0) {
            std::copy_n(first, fill_existing, ptrAt_(index));
        }
        for (size_type i = fill_existing; i < count; ++i) {
            std::construct_at(ptrAt_(size_ + i - fill_existing), *std::next(first, static_cast<difference_type>(i)));
        }

        size_ += count;
        return {ptrAt_(index), ErrorCode::Success};
    }

    /**
     * 在指定位置插入初始化列表
     * @param pos 插入位置
     * @param ilist 初始化列表
     * @return first 为插入起始迭代器，second 为错误码
     */
    constexpr std::pair<iterator, ErrorCode> insert(const_iterator pos, std::initializer_list<T> ilist) noexcept(
        std::is_nothrow_move_constructible_v<T> && std::is_nothrow_copy_assignable_v<T> &&
        std::is_nothrow_copy_constructible_v<T>)
    {
        return insert(pos, ilist.begin(), ilist.end());
    }

    // ---------- try_insert / try_emplace ----------

    /**
     * 尝试在指定位置插入单个元素
     * @param pos 插入位置
     * @param value 待插入元素
     * @return 插入位置迭代器，失败返回 nullptr
     */
    [[nodiscard]] constexpr iterator tryInsert(const_iterator pos, const value_type& value) noexcept(
        std::is_nothrow_move_constructible_v<T> && std::is_nothrow_copy_constructible_v<T>)
    {
        return insert(pos, value).first;
    }

    /**
     * @overload
     */
    [[nodiscard]] constexpr iterator tryInsert(const_iterator pos,
                                               value_type&& value) noexcept(std::is_nothrow_move_constructible_v<T>)
    {
        return insert(pos, std::move(value)).first;
    }

    /**
     * 尝试在指定位置原位构造
     * @tparam Args 构造参数类型
     * @param pos 插入位置
     * @param args 构造参数
     * @return 插入位置迭代器，失败返回 nullptr
     */
    template<typename... Args>
    [[nodiscard]] constexpr iterator tryEmplace(const_iterator pos,
                                                Args&&... args) noexcept(std::is_nothrow_move_constructible_v<T> &&
                                                                         std::is_nothrow_constructible_v<T, Args...>)
    {
        return emplace(pos, std::forward<Args>(args)...).first;
    }

    // ---------- insert_range / append_range ----------

    /**
     * 在指定位置插入范围
     * @tparam Range 范围类型
     * @param pos 插入位置
     * @param range 范围
     * @return first 为插入起始迭代器，second 为错误码
     */
    template<typename Range>
    constexpr std::pair<iterator, ErrorCode> insertRange(const_iterator pos, Range&& range) noexcept(
        noexcept(insert(pos, std::begin(range), std::end(range))))
    {
        return insert(pos, std::begin(range), std::end(range));
    }

    /**
     * 在尾部追加范围
     * @tparam Range 范围类型
     * @param range 范围
     * @retval ErrorCode::Success 成功
     * @retval ErrorCode::Full 容量不足
     */
    template<typename Range>
    constexpr ErrorCode appendRange(Range&& range) noexcept(
        noexcept(insert(cend(), std::begin(range), std::end(range))))
    {
        return insert(cend(), std::begin(range), std::end(range)).second;
    }

    /**
     * 尝试在尾部追加范围
     * @tparam Range 范围类型
     * @param range 范围
     * @return 是否成功
     */
    template<typename Range>
    [[nodiscard]] constexpr bool tryAppendRange(Range&& range) noexcept(
        noexcept(insert(cend(), std::begin(range), std::end(range))))
    {
        const auto r_size = static_cast<size_type>(std::distance(std::begin(range), std::end(range)));
        if (size_ + r_size > N) {
            return false;
        }
        appendRange(std::forward<Range>(range));
        return true;
    }

    // ---------- erase ----------

    /**
     * 移除指定位置元素
     * @param pos 待移除位置
     * @return 移除后下一个位置迭代器
     */
    constexpr iterator erase(const_iterator pos) noexcept(std::is_nothrow_move_assignable_v<T>)
    {
        return erase(pos, pos + 1);
    }

    /**
     * 移除范围 [first, last) 内元素
     * @param first 范围起始
     * @param last 范围结束
     * @return 移除后下一个位置迭代器
     */
    constexpr iterator erase(const_iterator first, const_iterator last) noexcept(std::is_nothrow_move_assignable_v<T>)
    {
        EMDEVIF_ASSERT(cbegin() <= first && first <= last && last <= cend(),
                       "[InplaceDynamicArray]: erase range out of range");

        const auto first_idx = static_cast<size_type>(std::distance(cbegin(), first));
        const auto last_idx = static_cast<size_type>(std::distance(cbegin(), last));

        if (first_idx == last_idx) {
            return ptrAt_(first_idx);
        }

        iterator new_end = std::move(ptrAt_(last_idx), ptrAt_(size_), ptrAt_(first_idx));
        std::destroy(new_end, ptrAt_(size_));
        size_ -= (last_idx - first_idx);
        return ptrAt_(first_idx);
    }

    // ---------- swap ----------

    /**
     * 交换两个数组内容
     * @param other 另一个数组
     */
    constexpr void swap(InplaceDynamicArray& other) noexcept(std::is_nothrow_swappable_v<T> &&
                                                             std::is_nothrow_move_constructible_v<T> &&
                                                             std::is_nothrow_move_assignable_v<T>)
    {
        using std::swap;
        const size_type common = (std::min)(size_, other.size_);

        for (size_type i = 0; i < common; ++i) {
            using std::swap;
            swap((*this)[i], other[i]);
        }

        if (size_ > other.size_) {
            for (size_type i = common; i < size_; ++i) {
                std::construct_at(other.ptrAt_(i), std::move((*this)[i]));
            }
            std::destroy(ptrAt_(common), ptrAt_(size_));
        }
        else if (other.size_ > size_) {
            for (size_type i = common; i < other.size_; ++i) {
                std::construct_at(ptrAt_(i), std::move(other[i]));
            }
            std::destroy(other.ptrAt_(common), other.ptrAt_(other.size_));
        }

        swap(size_, other.size_);
    }

    // ---------- 比较 ----------

    [[nodiscard]] friend constexpr auto operator<=>(const InplaceDynamicArray& a, const InplaceDynamicArray& b)
    {
        return std::lexicographical_compare_three_way(a.begin(), a.end(), b.begin(), b.end());
    }

    [[nodiscard]] friend constexpr bool operator==(const InplaceDynamicArray& a, const InplaceDynamicArray& b)
    {
        return a.size_ == b.size_ && std::equal(a.begin(), a.end(), b.begin());
    }
};

// ---------- 非成员 swap ----------

/**
 * 交换两个 InplaceDynamicArray
 * @tparam T 元素类型
 * @tparam N 容量
 * @param a 左操作数
 * @param b 右操作数
 */
template<typename T, std::size_t N>
constexpr void swap(InplaceDynamicArray<T, N>& a, InplaceDynamicArray<T, N>& b) noexcept(noexcept(a.swap(b)))
{
    a.swap(b);
}

// ---------- 非成员 erase / erase_if ----------

/**
 * 移除所有等于 value 的元素
 * @tparam T 元素类型
 * @tparam N 容量
 * @tparam U 比较值类型
 * @param c 容器
 * @param value 待移除值
 * @return 移除的元素数量
 */
template<typename T, std::size_t N, typename U>
constexpr typename InplaceDynamicArray<T, N>::size_type erase(InplaceDynamicArray<T, N>& c, const U& value)
{
    auto it = std::remove(c.begin(), c.end(), value);
    auto r = static_cast<typename InplaceDynamicArray<T, N>::size_type>(std::distance(it, c.end()));
    c.erase(it, c.end());
    return r;
}

/**
 * 移除满足谓词的元素
 * @tparam T 元素类型
 * @tparam N 容量
 * @tparam Pred 谓词类型
 * @param c 容器
 * @param pred 谓词
 * @return 移除的元素数量
 */
template<typename T, std::size_t N, typename Pred>
constexpr typename InplaceDynamicArray<T, N>::size_type eraseIf(InplaceDynamicArray<T, N>& c, Pred pred)
{
    auto it = std::remove_if(c.begin(), c.end(), pred);
    auto r = static_cast<typename InplaceDynamicArray<T, N>::size_type>(std::distance(it, c.end()));
    c.erase(it, c.end());
    return r;
}

}  // namespace emdevif

#endif  // !EMDEVIF_CORE_DATA_CONTAINER_INPLACE_DYNAMIC_ARRAY_HPP
