# peripheral AGENTS.md

## 模块定位
`emdevif_peripheral` 提供外设操作的统一接口。它不直接操作硬件，而是通过 `peripheral_handle_map` 机制将字符串名称映射到用户提供的外设实例，实现应用层与底层硬件的彻底解耦。

## 架构分层

```
peripheral/
├── model/                        # 外设模型层（平台无关的接口定义）
│   ├── inc/emdevif/peripheral/model/  # 头文件（gpio.hpp, serial.hpp, ...）
│   └── modules/                      # C++20 模块文件（gpio_model.cppm, ...）
├── peripheral_handle_map/        # 名称→实例的映射机制
│   ├── inc/emdevif/peripheral/       # peripheral_handle_map.hpp
│   └── PeripheralHandleMap.cppm
└── peripheral_impl_/             # 外设操作实现（对用户透明的具体类型）
    ├── inc/emdevif/peripheral/       # 头文件（gpio.hpp, serial.hpp, ...）
    └── Gpio.cppm, Serial.cppm, ...
```

### Model 层（`peripheral/model/`）
定义每种外设的抽象接口。每个外设包含：

- 一个 `XxxModel` 类，其中定义嵌套的 `Instance` 结构体
- `Instance` 是一组函数指针 + `void* handle` 的纯数据聚合，是用户侧与底层实现之间的契约

以 GPIO 为例：
```cpp
class GpioModel {
public:
    struct Instance {
        void* const handle{nullptr};
        const WriteFunction write_function{nullptr};
        const ReadFunction read_function{nullptr};
        const ToggleFunction toggle_function{nullptr};
    };
};
```

### Handle Map 层（`peripheral/peripheral_handle_map/`）
提供 `PeripheralHandleMap::findHandle(name)` 静态方法，委托给用户提供的 `emdevif::user_impl::peripheral_handle_map::findHandle` 函数。

- 用户需要在任意 `.cpp` 文件中实现 `emdevif::user_impl::peripheral_handle_map::findHandle(std::string_view) -> void*`
- 该函数负责将字符串名称映射到对应的 `XxxModel::Instance*`（返回 `void*`），未找到时返回 `nullptr`
- 不再要求 `constexpr`，不再使用 `makeStaticMap` / `array_map`
- 链接器在链接期自动解析符号

### Impl 层（`peripheral/peripheral_impl_/`）
每种外设的具体操作类（如 `Gpio` 继承 `GpioModel`）：

- 构造函数接受 `std::string_view name`，通过 `PeripheralHandleMap::findHandle` 查找实例
- 方法直接将调用委托给 `Instance` 中的函数指针
- 用户通过 `Gpio gpio("gpio1")` 即可使用，无需知道底层实现细节

## 数据流：从用户注册到外设调用

```
1. 用户在任意 .cpp 中:
   constinit GpioModel::Instance gpio1{ .handle = &hw_gpio, .write_function = ... };

   namespace emdevif::user_impl::peripheral_handle_map {
       void* findHandle(std::string_view name) noexcept {
           if (name == "gpio1") return &gpio1;
           return nullptr;
       }
   }

2. 应用代码:
   Gpio gpio("gpio1");           // 构造时调用 findHandle("gpio1") 查找实例
   gpio.write(Gpio::Set);        // 委托给 gpio1.write_function(hw_gpio, ...)
```

## 新增外设类型（以 I2C 为例）

### 1. 创建 Model
- 新建 `peripheral/model/inc/emdevif/peripheral/model/i2c.hpp`，定义 `I2cModel` 类和 `Instance` 结构体
- 新建 `peripheral/model/modules/i2c_model.cppm`，通过 `export module emdevif.peripheral.model.i2c` 导出
- Model 的 CMakeLists.txt 使用 `file(GLOB)` 自动收集 `.cppm`，无需手动修改

### 2. 创建 Impl
- 新建 `peripheral/peripheral_impl_/inc/emdevif/peripheral/i2c.hpp`，定义 `I2c` 类继承 `I2cModel`，构造函数接受 `string_view`
- 新建 `peripheral/peripheral_impl_/I2c.cppm`，导出 `emdevif.peripheral.i2c` 模块
- 在 `peripheral/peripheral_impl_/CMakeLists.txt` 的 `target_sources` 列表中添加 `I2c.cppm`

### 3. 更新依赖
- 确保 `peripheral_impl_` 链接了所需的其他模块（通常与现有外设一致即可）
- 不需要修改 `model/CMakeLists.txt`（GLOB 自动收集）

## 开发约束

- 用户实现 `findHandle` 的 `.cpp` 文件需要链接到最终可执行文件或库，以确保链接器能解析符号。
- Model 的 `Instance` 结构体必须保持 `standard_layout`（有 `static_assert` 验证）
- 函数指针签名变更属于破坏性变更，需同步更新所有平台适配层（如 `emdevif_stm32_peripheral`）
- 新增外设类型时，需同步考虑非 Modules 路径（`.hpp` 必须可独立于 `.cppm` 使用）
- 外设句柄名称（`findHandle` 的 `name` 参数）的语义要保持稳定，避免字符串名称冲突

## 参考资料
- 用户侧注册示例：`peripheral/README.md`
- 完整使用指南：`emdevif/README.md`
- 编码规范：https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev-dev/blob/main/docs/emdevif-coding-style.md
