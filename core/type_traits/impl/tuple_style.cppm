/**
 * @file tuple_style.cppm
 * @author DuYicheng
 * @date 2025-10-08
 * @brief 类型特征 - 元组相关
 */

module;

#include <type_traits>
#include <tuple>

export module emdevif.typeTraits:tupleStyle;

import boost.pfr;

namespace emdevif {

namespace internal {

template<typename T, typename = void>
struct is_tuple_like : std::false_type {
};

template<typename T>
struct is_tuple_like<T, std::void_t<decltype(std::tuple_size_v<T>), decltype(std::get<0>(std::declval<T>()))>>
    : std::true_type {
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
    static constexpr std::size_t tuple_size = get_tuple_size();
};

template<typename T, typename U>
class is_same_tuple_style
{
private:
    static constexpr bool both_are_tuple_style = is_tuple_style_v<T> && is_tuple_style_v<U>;
    static constexpr std::size_t nt = tuple_operations<T>::tuple_size;
    static constexpr std::size_t nu = tuple_operations<U>::tuple_size;

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
    template<typename T>
    using is_tuple_like = internal::is_tuple_like<T>;
    template<typename T>
    constexpr bool is_tuple_like_v = internal::is_tuple_like_v<T>;

    template<typename T>
    using is_tuple_style = internal::is_tuple_style<T>;
    template<typename T>
    constexpr bool is_tuple_style_v = internal::is_tuple_style_v<T>;

    template<typename T, typename U>
    using is_same_tuple_style = internal::is_same_tuple_style<T, U>;
    template<typename T, typename U>
    constexpr bool is_same_tuple_style_v = internal::is_same_tuple_style_v<T, U>;
}

namespace internal {

template<typename Agg, typename Tuple, std::size_t... I>
constexpr Agg tuple_to_agg_impl(Tuple&& t, std::index_sequence<I...>)
{
    return Agg{std::get<I>(std::forward<Tuple>(t))...};
}

}  // namespace internal

export
{
    template<typename Agg, typename Tuple>
        requires(std::is_aggregate_v<Agg> && internal::is_tuple_like_v<Tuple>)
    constexpr auto tuple_to_aggregate(Tuple && t)
    {
        constexpr std::size_t N = std::tuple_size_v<std::decay_t<Tuple>>;
        return internal::tuple_to_agg_impl<Agg>(std::forward<Tuple>(t), std::make_index_sequence<N>{});
    }

    template<typename Agg>
    constexpr auto aggregate_to_tuple(Agg && a)
    {
        return boost::pfr::structure_tie(std::forward<Agg>(a));
    }
}

}  // namespace emdevif
