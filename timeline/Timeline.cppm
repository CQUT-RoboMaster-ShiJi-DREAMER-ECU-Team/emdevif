/**
 * @file Timeline.cppm
 * @author DuYicheng
 * @date 2025-10-06
 * @brief 时间戳
 */

module;

#include <cstdint>

#include <type_traits>

export module emdevif.timeline;

import emdevif.userDeclares;

export namespace emdevif {

/// 时间点类型
using BaseTimePoint = uint64_t;

/**
 * 时间线类
 */
class Timeline
{
public:
    /**
     * 获取从初始化完成到当前的时间（以微秒为单位）
     * @return 当前时间
     */
    static BaseTimePoint getMicroseconds() noexcept
    {
        return ::emdevif::user_declares::timeline::getMicroseconds();
    }

    /**
     * 阻塞式延时一段时间
     * @param delay_time_us 要延时的时间（以微秒为单位）
     */
    static void pauseDelayUs(const BaseTimePoint delay_time_us) noexcept
    {
        const volatile auto begin = getMicroseconds();
        while (getMicroseconds() - begin < delay_time_us) {
        }
    }

    /**
     * 阻塞式延时一段时间
     * @param delay_time_ms 要延时的时间（以毫秒为单位）
     */
    static void pauseDelayMs(const BaseTimePoint delay_time_ms) noexcept
    {
        pauseDelayUs(delay_time_ms * 1000U);
    }

public:
    constexpr Timeline() noexcept = default;
    constexpr Timeline(const Timeline&) noexcept = default;

    explicit constexpr Timeline(const BaseTimePoint& init_value) noexcept : store_timeline_(init_value) {}

    Timeline& operator=(const BaseTimePoint& init_value) noexcept
    {
        store_timeline_ = init_value;
        return *this;
    }

    /**
     * 更新存储的时间
     */
    void update() noexcept
    {
        store_timeline_ = getMicroseconds();
    }

    /**
     * 获取存储的时间戳
     * @return 时间点值（以微秒为单位）
     */
    BaseTimePoint operator()() const noexcept
    {
        return store_timeline_;
    }

    template<typename OtherType>
        requires(std::is_convertible_v<BaseTimePoint, OtherType>)
    operator OtherType() noexcept  // NOLINT
    {
        return static_cast<OtherType>(store_timeline_);
    }

private:
    BaseTimePoint store_timeline_;  ///< 存储的时间点值（以微秒为单位）
};

/**
 * 间隔时间类
 * @tparam ConvertType 获取值时转化的类型
 */
template<typename ConvertType = float>
class Duration
{
public:
    constexpr Duration() noexcept = default;
    constexpr Duration(const Duration&) noexcept = default;

    template<typename OtherType>
        requires(requires(OtherType v) { Timeline{v}; })
    explicit constexpr Duration(const OtherType& last_time) noexcept : last_time_point_(last_time)
    {
    }

    /**
     * 获取间隔时间（以微秒为单位）
     * @return 间隔时间
     */
    ConvertType getMicroDuration() noexcept
    {
        const auto now = Timeline::getMicroseconds();
        const auto ret = now - last_time_point_();
        last_time_point_ = now;
        return ret;
    }

    /**
     * 获取间隔时间（以毫秒为单位）
     * @return 间隔时间
     */
    ConvertType getMilliDuration() noexcept
    {
        return getMicroDuration() / static_cast<ConvertType>(1000);
    }

    /**
     * 获取间隔时间（以秒为单位）
     * @return 间隔时间
     */
    ConvertType getSecondsDuration() noexcept
    {
        return getMilliDuration() / static_cast<ConvertType>(1000);
    }

    /**
     * 获取上一次时间的时间戳
     * @return 上一次的时间戳
     */
    ConvertType getLastTime() noexcept
    {
        return static_cast<ConvertType>(last_time_point_());
    }

    /**
     * 更新存储的时间戳
     */
    void update() noexcept
    {
        last_time_point_.update();
    }

private:
    Timeline last_time_point_;  ///< 上一次的时间戳
};

}  // namespace emdevif
