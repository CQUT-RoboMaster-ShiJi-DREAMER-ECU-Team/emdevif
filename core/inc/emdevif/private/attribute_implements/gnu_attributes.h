/**
 * @file gnu_attributes.h
 * @brief GNU 或 CCARM 的属性宏实现
 */

#ifndef EMDEVIF_GNU_ATTRIBUTES_H
#define EMDEVIF_GNU_ATTRIBUTES_H

#if ((!defined(__clang__)) && (defined(__GNUC__) || defined(__CC_ARM)))

/* clang-format off */

#ifdef __cplusplus
    #define EMDEVIF_NO_RETURN [[noreturn]]
#else
    #define EMDEVIF_NO_RETURN __attribute__((noreturn))
#endif

#ifdef __cplusplus
    #define EMDEVIF_MAYBE_UNUSED [[maybe_unused]]
#else
    #define EMDEVIF_MAYBE_UNUSED __attribute__((unused))
#endif

#ifdef __cplusplus
    #define EMDEVIF_USED [[gnu::used]]
#else
    #define EMDEVIF_USED __attribute__((used))
#endif

#ifdef __cplusplus
    #define EMDEVIF_WEAK [[gnu::weak]]
#else
    #define EMDEVIF_WEAK __attribute__((weak))
#endif

#ifdef __cplusplus
    #define EMDEVIF_ALWAYS_INLINE [[gnu::always_inline]]
#else
    #define EMDEVIF_ALWAYS_INLINE __attribute__((always_inline))
#endif

#ifdef __cplusplus
    #define EMDEVIF_FALL_THROUGH [[fall_through]]
#else
    #define EMDEVIF_FALL_THROUGH __attribute__((fallthrough))
#endif

#define EMDEVIF_SECTION(section_name) __attribute__((section(#section_name)))

#define EMDEVIF_FORMAT_CHECK(func, string_index, first_to_check) __attribute__((format(func, string_index, first_to_check)))

/* clang-format on */

#endif  // ((!defined(__clang__)) && (defined(__GNUC__) || defined(__CC_ARM)))

#endif  // !EMDEVIF_GNU_ATTRIBUTES_H
