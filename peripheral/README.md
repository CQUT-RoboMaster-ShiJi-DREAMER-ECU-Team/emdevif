# emdevif_peripheral

外设库，用于外设操作。

这个库依赖 emdevif_user_declares。

## emdevif_user_declares 的需求

**不可链接 `emdevif_peripheral` 库，否则导致循环依赖！**
但可以链接（一般情况下必然会链接） `emdevif_peripheral_model` 库。

**不可导入 `emdevif.peripheral.*`、`emdevif.peripheralHandleMap` 模块，否则导致循环依赖！**
但可以导入（一般情况下必然会导入）`emdevif.peripheral.model.*` 模块。

该模块需要用户在 emdevif_user_declares 中定义 `peripheral_handle_map` 变量：

```C++
#include <string_view>

import emdevif.container.arrayMap;  // makeStaticMap 函数需要
import emdevif.peripheral.model.serial;

import emdevif.stm32Peripheral.hal.usart;  // 示例使用的库

export namespace emdevif::user_declares {

constinit emdevif::SerialModel::Instance serial1{
    .handle = &huart1,
    .get_state_function = emdevif::stm32hal::uartGetState,
    .transmit_function = emdevif::stm32hal::uartTransmitInterrupt};

// 必须是常量表达式，变量名必须是 peripheral_handle_map
constexpr auto peripheral_handle_map =
    makeStaticMap<std::string_view, void*>({  // 模板参数必须是 <std::string_view, void*>
        {"serial1", &serial1},  // 填充外设表，将名称与模型的实例关联起来
    });

}  // namespace emdevif::user_declares
```
