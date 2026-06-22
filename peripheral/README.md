# emdevif_peripheral

外设库，用于外设操作。它不直接操作硬件，而是通过字符串名称映射到用户提供的外设实例，将应用层与底层硬件彻底解耦。

## 设计理念

emdevif 本身不实现任何外设驱动，只提供一套"名称查找 + 函数指针委托"的抽象框架。整个外设系统分三层：

- **Model 层**：定义每种外设的接口契约。它是一个纯数据聚合（`XxxModel::Instance`，包含 `void* handle` 和一组函数指针），与平台无关、与实现无关。
- **Handle Map 层**：维护一个编译期常量映射表（`string_view → void*`），将字符串名称关联到 `Instance` 的地址。
- **Impl 层**：对用户透明的包装类（如 `Gpio`），构造函数通过名称查找 `Instance`，方法直接委托给函数指针。

用户的职责只有一件事：在 `emdevif_user_declares` 中创建 `Instance` 并注册到 `peripheral_handle_map`。之后应用代码只需用字符串名称即可获取外设，无需知道底层是 STM32 HAL、ESP-IDF 还是其他平台。

## 应用层使用

每种外设都有一个同名的操作类（如 `Gpio`、`Serial`），通过字符串名称构造即可使用：

```cpp
import emdevif.peripheral.gpio;
import emdevif.peripheral.serial;

void app_main() {
    // 通过名称获取 GPIO 实例（名称需与 peripheral_handle_map 中一致）
    Gpio led("led_green");
    led.write(Gpio::Set);       // 点亮 LED
    led.toggle();               // 翻转

    // 通过名称获取串口实例
    Serial debug_uart("debug_console");
    const uint8_t msg[] = "Hello\r\n";
    debug_uart.transmit(false, msg, Serial::max_delay);
}
```

构造时如果名称在 `peripheral_handle_map` 中不存在，会触发断言失败。

各外设的完整 API 见对应头文件：

| 外设 | 头文件（Modules 关闭时） | 模块名（Modules 开启时） |
|---|---|---|
| GPIO | `emdevif/peripheral/gpio.hpp` | `emdevif.peripheral.gpio` |
| Serial | `emdevif/peripheral/serial.hpp` | `emdevif.peripheral.serial` |
| SPI | `emdevif/peripheral/spi.hpp` | `emdevif.peripheral.spi` |
| I2C | *待实现* | *待实现* |
| CAN | `emdevif/peripheral/can.hpp` | `emdevif.peripheral.can` |
| PWM | `emdevif/peripheral/pwm.hpp` | `emdevif.peripheral.pwm` |
| Timer | `emdevif/peripheral/timer.hpp` | `emdevif.peripheral.timer` |

## emdevif_user_declares 侧的注册方法

该库依赖用户在 `emdevif_user_declares` 中定义 `peripheral_handle_map` 变量。

**循环依赖警告：**
- `emdevif_user_declares` **不可链接** `emdevif_peripheral` 或 `emdevif_peripheral_impl_`
- 可链接（一般情况下必然会链接）`emdevif_peripheral_model`
- **不可导入** `emdevif.peripheral.*`、`emdevif.peripheralHandleMap` 模块
- 可导入（一般情况下必然会导入）`emdevif.peripheral.model.*` 模块

注册示例（以 STM32 HAL 串口为例）：

```cpp
#include <string_view>

import emdevif.core.data_container.array_map;  // makeStaticMap 函数需要
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

每个外设 Model 的 `Instance` 结构体字段见对应头文件（如 `emdevif/peripheral/model/gpio.hpp`）。

## 扩展新外设

详见 [peripheral/AGENTS.md](AGENTS.md)，里面有完整的架构说明和以 I2C 为例的添加步骤。
