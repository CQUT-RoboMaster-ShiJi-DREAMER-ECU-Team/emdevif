# emdevif_logger

日志库，提供同步和异步两种模式。

这个库依赖 emdevif_user_declares。

## 配置选项

参考 [emdevif_logger_config_example.hpp](./emdevif_logger_config_example.hpp)
文件，您可以将这个文件拷贝到您的工程中，并根据需要修改配置选项。然后在 CMakeLists.txt 中定义
`EMDEVIF_LOGGER_CONFIG_FILE` 宏来指定配置文件：

```CMake
target_compile_definitions(emdevif_logger PUBLIC
    EMDEVIF_LOGGER_CONFIG_FILE="path/to/your/emdevif_logger_config.hpp"
    # 注意：请确保宏定义的值被双引号包裹，从而让它被正确地解析为字符串字面量
)
```

## 依赖关系

异步模式时，依赖 `emdevif_system` 子模块。
同步模式时，如果启用了锁，则依赖 `emdevif_system` 子模块。

## emdevif_user_declares 的需求

**不可链接 `emdevif_logger` 库，否则导致循环依赖！**<br>
**不可导入 `emdevif.logger` 模块，否则导致循环依赖！**

该模块需要用户在 emdevif_user_declares 中声明 `getTimeLine`、`printLogMessage` 函数：

```C++
#include <cstddef>                  // for std::size_t
import emdevif.core.error_handler;  // for emdevif::ErrorCode

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

import emdevif.logger;

int main()
{
    emdevif::logger::init(std::vsnprintf);  // 需要提供一个 vsnprintf 风格的格式化函数

    while (true) {
        using namespace emdevif;

        logger::info("Hello, emdevif Logger!");  // 不需要换行符，会自动添加
        logger::info("Support printf-style %s!", "format");
        logger::debug(emdevif::LogLevel::Warning, "This is a debug log: %d", 42);
        // ...
    }
}
```
