/**
 * @file simplify_decl_macros.hpp
 * @author 杜以成
 * @date 2025-05-09
 * @brief 简化定义宏
 */

#pragma once
#ifndef RMDEV_SIMPLIFY_DECL_HPP
#define RMDEV_SIMPLIFY_DECL_HPP

/**
 * 删除拷贝构造函数和拷贝赋值运算符
 * @param ClassName 待删除的类名
 */
#define RMDEV_DELETE_COPY_CONSTRUCTOR(ClassName) \
    ClassName(const ClassName&) = delete;        \
    ClassName& operator=(const ClassName&) = delete

/**
 * 删除移动构造函数和移动赋值运算符
 * @param ClassName 待删除的类名
 */
#define RMDEV_DELETE_MOVE_CONSTRUCTOR(ClassName) \
    ClassName(ClassName&&) = delete;             \
    ClassName& operator=(const ClassName&&) = delete

/**
 * 删除拷贝与移动构造函数及相关运算符
 * @param ClassName 待删除的类名
 */
#define RMDEV_DELETE_COPY_AND_MOVE(ClassName) \
    RMDEV_DELETE_COPY_CONSTRUCTOR(ClassName); \
    RMDEV_DELETE_MOVE_CONSTRUCTOR(ClassName)

#endif  // !RMDEV_SIMPLIFY_DECL_HPP
