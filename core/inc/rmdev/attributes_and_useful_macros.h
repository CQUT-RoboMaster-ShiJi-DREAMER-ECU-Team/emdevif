/**
 * @file attributes_and_useful_macros.h
 * @author 杜以成
 * @date 2025-06-01
 * @brief 属性与常用宏
 */

#pragma once
#ifndef RMDEV_ATTRIBUTES_H
#define RMDEV_ATTRIBUTES_H

// 更简洁地实现 extern "C"
#ifdef __cplusplus
#define RMDEV_EXTERN_C       extern "C"
#define RMDEV_EXTERN_C_BEGIN extern "C" {
#define RMDEV_EXTERN_C_END   }
#else  // !__cplusplus
#define RMDEV_EXTERN_C_BEGIN
#define RMDEV_EXTERN_C_END
#define RMDEV_EXTERN_C
#endif  // !__cplusplus

/* Attributes ================================================================================== */
#include "private/attribute_implements/gnu_attributes.h"

#ifndef RMDEV_NO_RETURN
#error "RMDEV_NO_RETURN 属性未实现！"
#endif  // !RMDEV_NO_RETURN

#ifndef RMDEV_MAYBE_UNUSED
#error "RMDEV_MAYBE_UNUSED 属性未实现！"
#endif  // !RMDEV_MAYBE_UNUSED

#ifndef RMDEV_USED
#error "RMDEV_USED 属性未实现！"
#endif

#ifndef RMDEV_WEAK
#error "WEAK 属性未实现！"
#endif  // !RMDEV_WEAK

#ifndef RMDEV_ALWAYS_INLINE
#error "RMDEV_ALWAYS_INLINE 属性未实现！"
#endif  // !RMDEV_ALWAYS_INLINE

#ifndef RMDEV_FALL_THROUGH
#error "RMDEV_FALL_THROUGH 属性未实现！"
#endif  // !RMDEV_FALL_THROUGH

/* Useful Macros =============================================================================== */

// 通过将变量强制转换为 void 类型来实现取消编译器警告
#define RMDEV_UNUSED(X)      (void)X

/// 计算 C 风格数组的元素个数
#define RMDEV_COUNTOF(array) (sizeof(array) / sizeof(array[0]))

#endif  // !RMDEV_ATTRIBUTES_H
