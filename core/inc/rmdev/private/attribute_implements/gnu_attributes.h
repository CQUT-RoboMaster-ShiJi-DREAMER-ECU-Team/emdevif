/**
 * @file gnu_attributes.h
 * @author 杜以成
 * @date 2025-02-05
 * @brief GNU 或 CCARM 的属性宏实现
 */

#ifndef RMDEV_GNU_ATTRIBUTES_H
#define RMDEV_GNU_ATTRIBUTES_H

#if (defined(__GNUC__) || defined(__CC_ARM))

/* clang-format off */

#ifdef __cplusplus
    #define RMDEV_NO_RETURN [[noreturn]]
#else
    #define RMDEV_NO_RETURN __attribute__((noreturn))
#endif

#ifdef __cplusplus
    #define RMDEV_MAYBE_UNUSED [[maybe_unused]]
#else
    #define RMDEV_MAYBE_UNUSED __attribute__((unused))
#endif

#ifdef __cplusplus
    #define RMDEV_USED [[gnu::used]]
#else
    #define RMDEV_USED __attribute__((used))
#endif

#ifdef __cplusplus
    #define RMDEV_WEAK [[gnu::weak]]
#else
    #define RMDEV_WEAK __attribute__((weak))
#endif

#ifdef __cplusplus
    #define RMDEV_ALWAYS_INLINE [[gnu::always_inline]]
#else
    #define RMDEV_ALWAYS_INLINE __attribute__((always_inline))
#endif

#ifdef __cplusplus
    #define RMDEV_FALL_THROUGH [[fall_through]]
#else
    #define RMDEV_FALL_THROUGH __attribute__((fallthrough))
#endif

/* clang-format on */

#endif  // (defined(__GNUC__) || defined(__CC_ARM))

#endif  // !RMDEV_GNU_ATTRIBUTES_H
