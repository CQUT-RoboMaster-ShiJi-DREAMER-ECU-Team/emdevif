# emdevif_timeline

微秒级时间戳以及间隔时间测量。

这个库依赖 emdevif_user_declares。

## 配置

* 宏 `EMDEVIF_TIMELINE_SOURCE_IS_MILLISECONDS`: 如果定义为 true，则 `Timeline` 最高精度为毫秒。默认为 false。

## emdevif_user_declares 的需求

**不可链接 `emdevif_timeline` 库，否则导致循环依赖！**<br>
**不可导入 `emdevif.timeline` 模块，否则导致循环依赖！**

该模块需要用户在 emdevif_user_declares 中声明 `getMicroseconds` 函数：

```C++
#include <cstdint>  // for uint64_t

export namespace emdevif::user_declares {

// 实现在命名空间 emdevif::user_declares::timeline 内
namespace timeline {

// 用于获取以微秒为单位的时间戳
uint64_t getMicroseconds()  // 可选 inline、noexcept 修饰符
{
    // 返回以微秒为单位的时间戳
}

// 如果 EMDEVIF_TIMELINE_SOURCE_IS_MILLISECONDS 定义为 true，则应该改为提供 getMilliseconds 函数，返回以毫秒为单位的时间戳
// uint64_t getMilliseconds()
// {
//     // 返回以毫秒为单位的时间戳
// }

}  // namespace timeline

}  // namespace emdevif::user_declares
```

## 示例

```C++
#include <iostream>
import emdevif.timeline;

int main()
{
    using namespace emdevif;

    Timeline tl;
    std::cout << tl() << "\n";  // 由于 tl 未初始化，因此输出的值未知

    tl.update();  // 更新 tl 存储的时间戳
    std::cout << tl() << "\n";  // 输出执行上一行时的时间戳（以微秒为单位）

    Timeline::pauseDelayMs(5);  // 阻塞式延时 5 毫秒（用空循环实现）

    Duration duration;
    duration.update();  // 更新间隔时间
    while (true) {
        // ...

        auto d1 = duration.getMicroDuration();
        // 获取间隔时间（从上次调用 duration.update() 或 duration.getMicroDuration() 开始计时）

        auto d2 = duration.getSecondsDuration();
        // 获取间隔时间（以秒为单位。如果 Duration 的模板参不是浮点类型，有可能损失精度）
    }
}
```
