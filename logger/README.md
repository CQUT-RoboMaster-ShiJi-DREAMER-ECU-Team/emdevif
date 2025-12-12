# emdevif_logger

日志库，提供同步和异步两种模式。

这个库依赖 emdevif_user_declares。

## 配置选项

| 宏                                      | 默认值                         | 含义                                                                                                                                                                                                                                       |
|----------------------------------------|-----------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| EMDEVIF_LOGGER_BUFFER_SIZE             | `256U`                      | 一个日志缓冲区大小                                                                                                                                                                                                                                |
| EMDEVIF_LOGGER_BUFFER_COUNT            | `4`                         | 日志缓冲区数量                                                                                                                                                                                                                                  |
| EMDEVIF_LOGGER_MODE                    | `EMDEVIF_LOGGER_MODE_ASYNC` | 日志模式，支持同步和异步两种模式。可选值：<br> - `EMDEVIF_LOGGER_MODE_SYNC`：同步模式<br> - `EMDEVIF_LOGGER_MODE_ASYNC`：异步模式                                                                                                                                       |
| EMDEVIF_LOGGER_SYNC_USE_LOCK           | `true`                      | 在同步模式下（异步模式下无效），是否使用锁来保护日志输出。设置为 `true` 将启用锁，`false` 则不启用锁。                                                                                                                                                                              |
| EMDEVIF_LOGGER_ASYNC_THREAD_STACK_SIZE | `128`                       | 异步模式下（同步模式下无效）的日志线程栈大小。                                                                                                                                                                                                                  |
| EMDEVIF_LOGGER_IGNORE_LEVEL            | `LogLevel::Debug`           | 忽略的日志等级。<br>日志等级由低到高依次为：`LogLevel::Debug`, `LogLevel::Info`, `LogLevel::Warning`, `LogLevel::Error`, `LogLevel::Fatal`。<br>如果日志等级低于这个忽略等级，那么该日志将会被忽略掉。例如设置的忽略等级是 `LogLevel::Warning`，那么 `LogLevel::Debug`和`LogLevel::Info` 会被忽略，其他的会被输出。 |

例如：

```CMake
target_compile_definitions(emdevif_logger PUBLIC EMDEVIF_LOGGER_BUFFER_SIZE=128)
```

## 依赖关系

异步模式时，依赖 `emdevif_system` 子模块。
同步模式时，如果启用了锁，则依赖 `emdevif_system` 子模块。

## emdevif_user_declares 的需求

**不可链接 `emdevif_logger` 库，否则导致循环依赖！**<br>
**不可导入 `emdevif.logger` 模块，否则导致循环依赖！**

该模块需要用户在 emdevif_user_declares 中声明 `getTimeLine`、`printLogMessage` 函数：

```C++
#include <cstddef>            // for std::size_t
import emdevif.errorHandler;  // for emdevif::ErrorCode

export namespace emdevif::user_declares {

// 实现在 emdevif::user_declares::logger 命名空间中声明
namespace logger {

// 用于获取当前时间戳的函数
std::size_t getTimeLine()  // 可选 inline、noexcept 等修饰符
{
    // 返回时间戳，单位可以是毫秒、微秒等，取决于具体实现
}

// 用于打印日志消息的函数
// message: 要打印的日志消息，已经格式化完成
ErrorCode printLogMessage(const char* message)
{
    // 在这里实现日志消息的打印逻辑，例如输出到控制台、文件、串口外设等

    // 返回 emdevif::ErrorCode::Success 表示成功，其他值表示失败
}

}  // namespace logger

}  // namespace emdevif::user_declares
```

## 示例

```C++
#include <cstdio>
#include "emdevif/logger.hpp"

import emdevif.logger;

int main()
{
    emdevif::Logger::init();
    emdevif::Logger::registerVSPrintfFunction(std::vsprintf);  // 如果不调用这个函数，默认使用 std::vsprintf

    while (true) {
        EMDEVIF_LOG(emdevif::LogLevel::Info, "Hello, emdevif Logger!");  // 不需要换行符，会自动添加
        EMDEVIF_LOG(emdevif::LogLevel::Info, "Support printf-style %s!", "format");

        EMDEVIF_DLOG(emdevif::LogLevel::Warning, "This is a debug log: %d", 42);  // 调试日志，仅在调试模式下（未定义 NDEBUG 宏时）输出
        // ...
    }
}
```
