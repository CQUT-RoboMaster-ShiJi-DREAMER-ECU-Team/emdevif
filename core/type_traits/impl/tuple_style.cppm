/**
 * @file tuple_style.cppm
 * @author DuYicheng
 * @date 2025-10-08
 * @brief 类型特征 - 元组相关
 */

module;

#include <type_traits>
#include <utility>
#include <tuple>
#include <array>

export module emdevif.typeTraits:tupleStyle;

import boost.pfr;

namespace emdevif {

namespace internal {

/**
 * 判断是否为 std::array 类型
 *
 * 存在简化表达式 @ref is_std_array_v
 * @tparam T 待判断的类型
 */
template<typename T>
struct is_std_array : std::false_type {
};
/// @copydoc is_std_array
template<typename U, std::size_t N>
struct is_std_array<std::array<U, N>> : std::true_type {
};

/**
 * 判断是否为 std::array 类型 - 简化表达式
 * @tparam T 待判断的类型
 */
template<typename T>
inline constexpr bool is_std_array_v = is_std_array<T>::value;

template<typename T>
struct is_tuple_like : std::false_type {
};
template<typename T, typename U>
struct is_tuple_like<std::pair<T, U>> : std::true_type {
};
template<typename... Ts>
struct is_tuple_like<std::tuple<Ts...>> : std::true_type {
};

template<typename T>
constexpr bool is_tuple_like_v = is_tuple_like<T>::value;

template<typename T>
class is_tuple_style
{
private:
    static constexpr bool is_aggregate_or_scalar = std::is_aggregate_v<T> || std::is_scalar_v<T>;

    static constexpr bool is_boost_pfr_tuple_like() noexcept
    {
        if constexpr (!is_aggregate_or_scalar) {
            return false;
        }
        else {
            return (requires { boost::pfr::tuple_size_v<T>; } && requires { boost::pfr::get<0>(std::declval<T>()); });
        }
    }

    static constexpr bool result() noexcept
    {
        if constexpr (is_tuple_like_v<T>) {
            return true;
        }
        else {
            return is_boost_pfr_tuple_like();
        }
    }

public:
    static constexpr bool value = result();
};

template<typename T>
constexpr bool is_tuple_style_v = is_tuple_style<T>::value;

// 根据类型选择使用 std 还是 boost::pfr 的 API
// （std::tuple_size_v、std::tuple_element_t 只能用于 std::pair 或者 std::tuple，而
// boost::pfr 中同名函数只能用于聚合类型，因此通过这个类的包装以自动调用不同的函数）
template<typename T>
    requires(is_tuple_style_v<T>)
class tuple_operations
{
public:
    template<std::size_t I>
    class tuple_element
    {
    private:
        template<typename T_, typename = void>
        struct operation;

        template<typename T_>
        struct operation<T_, std::enable_if_t<is_tuple_like_v<T_>>> {
            using type = std::tuple_element_t<I, T_>;
        };

        template<typename T_>
        struct operation<T_, std::enable_if_t<!is_tuple_like_v<T_>>> {
            using type = boost::pfr::tuple_element_t<I, T_>;
        };

    public:
        using type = typename operation<T>::type;
    };
    template<std::size_t I>
    using tuple_element_t = typename tuple_element<I>::type;

private:
    static constexpr std::size_t get_tuple_size() noexcept
    {
        if constexpr (is_tuple_like_v<T>) {
            return std::tuple_size_v<T>;
        }
        else {
            return boost::pfr::tuple_size_v<T>;
        }
    }

public:
    static constexpr std::size_t tuple_size_v = get_tuple_size();
};

template<typename T, typename U>
class is_same_tuple_style
{
private:
    static constexpr bool both_are_tuple_style = is_tuple_style_v<T> && is_tuple_style_v<U>;
    static constexpr std::size_t nt = tuple_operations<T>::tuple_size_v;
    static constexpr std::size_t nu = tuple_operations<U>::tuple_size_v;

    template<typename T_, typename U_, std::size_t... I>
    static constexpr bool is_same_type_for_each_member(std::index_sequence<I...>) noexcept
    {
        return ((std::is_same_v<typename tuple_operations<T_>::tuple_element_t<I>,
                                typename tuple_operations<U_>::tuple_element_t<I>>) &&
                ...);
    }

public:
    static constexpr bool value =
        both_are_tuple_style && nt == nu && is_same_type_for_each_member<T, U>(std::make_index_sequence<nt>{});
};

template<typename T, typename U>
constexpr bool is_same_tuple_style_v = is_same_tuple_style<T, U>::value;

}  // namespace internal

export
{
    /**
     * @page diff_tuple_like_and_tuple_style
     * **类似元组（tuple_like）** 与 **元组形式（tuple_style）** 的区别
     *     @arg 类似元组：只能是 std::tuple 或 std::pair
     *     @arg 元组形式：struct { T1 t1; T2 t2; } 或 std::array<int, 2> 也是元组形式
     */

    /**
     * 判断是否为类似元组（std::tuple 或 std::pair）类型
     *
     * @copydoc diff_tuple_like_and_tuple_style
     *
     * 存在简化表达式 @ref is_tuple_like_v
     * @tparam T 待判断的类型
     */
    template<typename T>
    using is_tuple_like = internal::is_tuple_like<T>;

    /**
     * 判断是否为类似元组（std::tuple 或 std::pair）类型 - 简化表达式
     *
     * @copydoc diff_tuple_like_and_tuple_style
     *
     * 是 @ref is_tuple_like 的简化表达式
     * @tparam T 待判断的类型
     */
    template<typename T>
    constexpr bool is_tuple_like_v = internal::is_tuple_like_v<T>;

    /**
     * 判断是否为元组形式的类型
     *
     * 当一个类型 **T** 满足以下条件时，是元组形式的类型（即 is_tuple_style<T>::value 为 true）：
     *     @arg @ref is_tuple_like_v <T> 为 true 时，或
     *     @arg **T** 是聚合类型
     *
     * @copydoc diff_tuple_like_and_tuple_style
     *
     * 存在简化表达式 @ref is_tuple_style_v
     * @tparam T 待判断的类型
     */
    template<typename T>
    using is_tuple_style = internal::is_tuple_style<T>;

    /**
     * 判断是否为元组形式的类型 - 简化表达式
     *
     * 元组形式的类型的定义见 @ref is_tuple_style
     * @tparam T 待判断的类型
     */
    template<typename T>
    constexpr bool is_tuple_style_v = internal::is_tuple_style_v<T>;

    /**
     * 判断两个元组形式的类型是否是形式相等的
     *
     * 如果两个类型的排布方式相同，认为是形式相等的，例如：
     *     @arg struct T { int a; char b; } 和 using U = std::pair<int, char>
     *          (T、U 的元素个数相等且类型顺序相同)
     *     @arg using T = std::array<float, 5> 和 using U = std::tuple<float,float,float,float,float>
     *          (T、U 的元素个数相等且类型顺序相同。但请注意，T 不能是 C 风格数组)
     *
     * 存在简化表达式 @ref is_same_tuple_style_v
     * @tparam T 待判断的第一个类型
     * @tparam U 待判断的第二个类型
     */
    template<typename T, typename U>
    using is_same_tuple_style = internal::is_same_tuple_style<T, U>;

    /**
     * 判断两个元组形式的类型是否是形式相等的 - 简化表达式
     *
     * 两个类型是形式相等的说明见 @ref is_same_tuple_style
     * @tparam T 待判断的第一个类型
     * @tparam U 待判断的第二个类型
     */
    template<typename T, typename U>
    constexpr bool is_same_tuple_style_v = internal::is_same_tuple_style_v<T, U>;
}

namespace internal {

template<typename Agg, typename Tuple, std::size_t... I>
constexpr Agg tuple_to_agg_impl(Tuple&& t, std::index_sequence<I...>) noexcept
{
    return Agg{std::get<I>(std::forward<Tuple>(t))...};
}

}  // namespace internal

export
{
    /* clang-format off */

    /**
     * 类似元组类型转化为聚合类型
     * @tparam Agg 目标聚合类型
     * @tparam Tuple 待转化的类似元组类型，一般可以自动推导，无需手动传入
     * @param t 待转化的类似元组类型变量
     * @return 转化成聚合类型的目标
     */
    template<typename Agg, typename Tuple>
        requires(std::is_aggregate_v<Agg> && internal::is_tuple_like_v<std::decay_t<Tuple>>)
    constexpr auto tuple_to_aggregate(Tuple&& t) noexcept
    {
        constexpr std::size_t N = std::tuple_size_v<std::decay_t<Tuple>>;
        return internal::tuple_to_agg_impl<Agg>(std::forward<Tuple>(t), std::make_index_sequence<N>{});
    }

    /**
     * 聚合类型转化为 std::tuple 类型
     * @tparam Agg 待转化的聚合类型，一般可以自动推导，无需手动传入
     * @param a 待转化的聚合类型变量
     * @return 转化成元组类型的目标
     */
    template<typename Agg>
    constexpr auto aggregate_to_tuple(const Agg& a) noexcept
    {
        return boost::pfr::structure_to_tuple(a);
    }

    /**
     * 聚合类型转化为 std::pair 类型
     * @tparam Agg 待转化的聚合类型，一般可以自动推导，无需手动传入
     * @param a 待转化的聚合类型变量
     * @return 转化成 std::pair 类型的目标
     */
    template<typename Agg>
        requires (boost::pfr::tuple_size_v<std::decay_t<Agg>> == 2)
    constexpr auto aggregate_to_pair(const Agg& a) noexcept
    {
        return std::make_pair(boost::pfr::get<0>(a), boost::pfr::get<1>(a));
    }
    /* clang-format on */
}

}  // namespace emdevif
