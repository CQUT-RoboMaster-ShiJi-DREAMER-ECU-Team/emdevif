/**
 * @file attributes_and_useful_macros.h
 * @author 杜以成
 * @date 2025-06-01
 * @brief 属性与常用宏
 */

#pragma once
#ifndef EMDEVIF_ATTRIBUTES_H
#define EMDEVIF_ATTRIBUTES_H

// 更简洁地实现 extern "C"
#ifdef __cplusplus
#define EMDEVIF_EXTERN_C       extern "C"
#define EMDEVIF_EXTERN_C_BEGIN extern "C" {
#define EMDEVIF_EXTERN_C_END   }
#else  // !__cplusplus
#define EMDEVIF_EXTERN_C_BEGIN
#define EMDEVIF_EXTERN_C_END
#define EMDEVIF_EXTERN_C
#endif  // !__cplusplus

/* Attributes ================================================================================== */
#include "private/attribute_implements/gnu_attributes.h"

#ifndef EMDEVIF_NO_RETURN
#error "EMDEVIF_NO_RETURN 属性未实现！"
#endif  // !EMDEVIF_NO_RETURN

#ifndef EMDEVIF_MAYBE_UNUSED
#error "EMDEVIF_MAYBE_UNUSED 属性未实现！"
#endif  // !EMDEVIF_MAYBE_UNUSED

#ifndef EMDEVIF_USED
#error "EMDEVIF_USED 属性未实现！"
#endif

#ifndef EMDEVIF_WEAK
#error "WEAK 属性未实现！"
#endif  // !EMDEVIF_WEAK

#ifndef EMDEVIF_ALWAYS_INLINE
#error "EMDEVIF_ALWAYS_INLINE 属性未实现！"
#endif  // !EMDEVIF_ALWAYS_INLINE

#ifndef EMDEVIF_FALL_THROUGH
#error "EMDEVIF_FALL_THROUGH 属性未实现！"
#endif  // !EMDEVIF_FALL_THROUGH

/* Useful Macros =============================================================================== */

// 通过将变量强制转换为 void 类型来实现取消编译器警告
#define EMDEVIF_UNUSED(X)      (void)X

/// 计算 C 风格数组的元素个数
#define EMDEVIF_COUNTOF(array) (sizeof(array) / sizeof(array[0]))

#endif  // !EMDEVIF_ATTRIBUTES_H
