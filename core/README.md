# emdevif_core

核心模块，所有库都需要它。

提供两个 CMake 目标：`emdevif_core` 和 `emdevif_compile_options`。前者是核心模块的静态库目标，
其他所有模块都需要链接它（以 PUBLIC 的方式）；后者是一个接口库目标，包含了一些编译选项（主要是配置编译警告），
其他 emdevif 的子模块也需要链接它（以 PRIVATE 的方式），而用户或第三方库的目标可以选择性地链接。

## 主要功能

### 概览

* 基本头文件（这些功能仅提供头文件，没有模块）：提供基础的数据类型定义和宏定义。
    * [attributes_and_useful_macros.h](inc/emdevif/core/attributes_and_useful_macros.h): 提供常用的属性和宏定义。
    * [line_separator.h](inc/emdevif/core/line_separator.h): 提供统一的换行符的定义。
    * [simplify_decl_macros.hpp](inc/emdevif/core/simplify_decl_macros.hpp): 提供简化声明的宏。
  若头文件的后缀名是 `.h`，则表示它们在 C 和 C++ 中都可以使用；若后缀名是 `.hpp`，则表示它们只能在 C++ 中使用。
* [错误处理模块](modules/error_handler.cppm): 提供错误码定义和错误处理函数。
* [数据容器](modules/data_container): 提供常用的数据容器实现（如环形缓冲区）。
* [integer_suffix.cppm](modules/integer_suffix.cppm): 提供固定宽度整型、size_t、ptrdiff_t 的自定义字面量。
* STL 库的扩展：
    * [类型特征](modules/type_traits.cppm): 提供部分类型特征的实现。
    * [概念库](modules/concepts.cppm): 提供一些常用概念的实现。
    * [端序](modules/endian.cppm): 提供端序转换相关的功能。
    * [资源保护](modules/resource_guard.cppm): 实现安全释放资源的机制（如 LockGuard、轻量级 try-finally）。

### 详细功能说明

此处仅列举需要用户增加配置才能正常使用，或者比较复杂的功能。其他功能比较简单，无需用户配置的功能参考各自的文件内注释即可。

#### CMake 的 emdevif_compile_options 接口库

这个库目前只有一个功能：编译时启用所有警告。emdevif 的所有子模块都会链接它（以 PRIVATE 的方式）。所有 emdevif 的内部实现都能够确保无警告。
但考虑到用户或第三方库的代码不一定能确保无警告，因此将开启所有警告的选项打包成接口库，用户可以自行决定是否链接它以开启所有警告。

#### line_separator.h

您可以设置宏 `EMDEVIF_LINE_SEPARATOR_MODE` 的值以选择换行符的风格：

| 值                                       | 含义                    |
|-----------------------------------------|-----------------------|
| `EMDEVIF_LINE_SEPARATOR_MODE_CRLF`（默认值） | 使用 CRLF 作为换行符（`\r\n`） |
| `EMDEVIF_LINE_SEPARATOR_MODE_LF  `      | 使用 LF 作为换行符（`\n`）     |
| `EMDEVIF_LINE_SEPARATOR_MODE_CR  `      | 使用 CR 作为换行符（`\r`）     |

例如：
```CMake
target_compile_definitions(emdevif_core PUBLIC EMDEVIF_LINE_SEPARATOR_MODE=EMDEVIF_LINE_SEPARATOR_MODE_LF)
```

然后就能使用宏 `EMDEVIF_LINE_SEPARATOR` 来表示换行符了，例如：

```C
#include <stdio.h>
#include "emdevif/line_separator.h"

int main(void)
{
    printf("Hello, World!" EMDEVIF_LINE_SEPARATOR);
    // EMDEVIF_LINE_SEPARATOR 是字符串常量，表达式 "Hello, World!" EMDEVIF_LINE_SEPARATOR
    // 等价于 "Hello, World!\r\n"（假设使用默认值 CRLF）

    return 0;
}
```

#### 错误处理模块

错误处理模块主要提供两个功能：错误码类型定义以及致命错误处理。

致命错误处理由以下函数、宏实现：

* `emdevif::terminate` 函数: 立刻停止程序，默认实现是进入死循环。您可以通过调用 `emdevif::registerTerminateFunction`
  函数以注册自定义的终止函数（例如关闭中断后进入死循环）。被注册的函数必须确保不会返回，否则属于未定义行为。
* `EMDEVIF_FATAL_HANDLER` 宏: 用于在发生致命错误时调用的宏。您可以通过调用
  `emdevif::registerFatalHandler` 函数以注册自定义的致命错误处理函数（例如向串口发送一条信息）。被注册的函数可以返回，
  因为执行了用户注册的函数后会调用 `emdevif::terminate` 函数。
* `EMDEVIF_ASSERT` 宏: 用于在调试时（未定义 `NDEBUG` 宏时）断言检查的宏，如果条件不满足则触发断言失败处理函数。您可以调用
  `emdevif::registerAssertFailedHandler`
  函数以注册自定义的断言失败处理函数。被注册的函数可以返回，因为执行了用户注册的函数后会调用 `emdevif::terminate` 函数。

`EMDEVIF_ASSERT` 与 `EMDEVIF_FATAL_HANDLER` 的区别是：前者允许通过设置宏 `NDEBUG` 来禁用断言检查，而后者则始终有效。

使用错误处理函数时，需要导入 `emdevif.core.error_handler` 模块。若需要使用宏，还需要导入头文件 `emdevif/fatal_handler.h`。
特别地，在 C 语言中使用宏，只需要导入头文件 `emdevif/fatal_handler.h` 即可。
