/**
 * @file simplify_decl_macros.hpp
 * @author 杜以成
 * @date 2025-05-09
 * @brief 简化定义宏
 */

#pragma once
#ifndef EMDEVIF_SIMPLIFY_DECL_HPP
#define EMDEVIF_SIMPLIFY_DECL_HPP

/**
 * 删除拷贝构造函数和拷贝赋值运算符
 * @param ClassName 待删除的类名
 */
#define EMDEVIF_DELETE_COPY_CONSTRUCTOR(ClassName) \
    ClassName(const ClassName&) = delete;          \
    ClassName& operator=(const ClassName&) = delete

/**
 * 删除移动构造函数和移动赋值运算符
 * @param ClassName 待删除的类名
 */
#define EMDEVIF_DELETE_MOVE_CONSTRUCTOR(ClassName) \
    ClassName(ClassName&&) = delete;               \
    ClassName& operator=(const ClassName&&) = delete

/**
 * 删除拷贝与移动构造函数及相关运算符
 * @param ClassName 待删除的类名
 */
#define EMDEVIF_DELETE_COPY_AND_MOVE(ClassName) \
    EMDEVIF_DELETE_COPY_CONSTRUCTOR(ClassName); \
    EMDEVIF_DELETE_MOVE_CONSTRUCTOR(ClassName)

/**
 * 声明为零例模式
 * @param ClassName 类名
 */
#define EMDEVIF_DECLARE_ZERO_INSTANCE(ClassName) \
    ClassName() = delete;                        \
    ~ClassName() = delete;                       \
    EMDEVIF_DELETE_COPY_AND_MOVE(ClassName)

#endif  // !EMDEVIF_SIMPLIFY_DECL_HPP
