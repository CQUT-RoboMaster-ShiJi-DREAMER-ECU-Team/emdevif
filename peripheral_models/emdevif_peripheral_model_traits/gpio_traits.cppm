/**
 * @file gpio_traits.cppm
 * @author DuYicheng
 * @date 2025-10-07
 * @brief GPIO 类型特征
 */

module;

#include <cstdint>

#include <type_traits>
#include <tuple>

export module emdevif.peripheralModels.gpioTraits;

import emdevif.typeTraits;

export namespace emdevif {

template<typename T>
struct is_gpio_handle {
    static constexpr bool value = emdevif::is_same_tuple_style_v<T, std::tuple<void*, uint32_t>>;
};

template<typename T>
constexpr bool is_gpio_handle_v = is_gpio_handle<T>::value;

}  // namespace emdevif
