# emdevif 测试

使用了 [emdevif_test_framework](https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/emdevif_test_framework.git)
测试框架。

测试内容在 [test_suits](test_suits) 中，文件结构与 emdevif 中的类似。

`test_main.cpp` 中，声明了一个 `extern "C" void emdevif_testEntry(void)` 函数，用于执行测试；`test_main.hpp`
存放测试套件中的入口函数的声明。不提供外部头文件，您需要在调用处手动使用 extern 声明，例如：

```C++
#include <cstdio>
#include <cstdarg>
#include "emdevif_test_framework.h"

extern "C" void emdevif_testEntry(void);  // 不提供头文件，您需要手动在这里声明
// 如果是在 C 语言中声明，需要去掉 extern "C"

int main()
{
    constexpr emdevif_test_Callbacks callbacks{
        .printfCallback = [](const char* format, ...) -> void {
            std::va_list args;
            va_start(args, format);
            std::vprintf(format, args);
            va_end(args);
        },
        .testEntryCallback = emdevif_testEntry,
        .errorCallback = nullptr};
    emdevif_test_framework_main(EMDEVIF_LINE_SEPARATOR, &callbacks, nullptr);
}
```

## 配置

CMake 缓存变量：

* `EMDEVIF_TEST_IGNORE_NAME_LIST`: 要忽略的测试列表，用分号间隔开。要忽略的测试名称的取值见
  [emdevif_test_list.hpp](emdevif_test_list.hpp) 中 `emdevif_test_list` 变量的定义。
