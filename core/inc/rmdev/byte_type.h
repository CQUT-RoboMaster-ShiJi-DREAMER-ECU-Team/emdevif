/**
 * @file byte_type.h
 * @author 杜以成
 * @date 2025-01-29
 * @brief 字节类型（一般用于 void* 类型的自增运算或者静态内存分配）
 */

#ifndef RMDEV_BYTE_TYPE_H
#define RMDEV_BYTE_TYPE_H

#ifdef __cplusplus
extern "C" {
#else
#include <assert.h>
#endif

typedef signed char byte_t;     ///< 有符号字节类型
typedef unsigned char ubyte_t;  ///< 无符号字节类型

static_assert(sizeof(byte_t) == 1U, "字节类型大小不匹配！");
static_assert(sizeof(ubyte_t) == 1U, "字节类型大小不匹配！");

#ifdef __cplusplus
}
#endif

#endif  // !RMDEV_BYTE_TYPE_H
