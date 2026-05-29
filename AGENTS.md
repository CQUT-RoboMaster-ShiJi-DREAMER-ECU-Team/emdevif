# AGENTS.md

## 模块定位
`emdevif` 提供嵌入式开发通用接口抽象（core/logger/timeline/peripheral/system/util），并作为 `rmdev` 的底层基础能力。

## 仓库结构

```
emdevif/
├── core/                       # 必选基础层（错误处理、断言、C/C++ 边界宏、depends/）
├── logger/                     # 日志模块
├── timeline/                   # 微秒级时间戳
├── peripheral/                 # 外设操作接口（model / handle_map / impl_，有独立 AGENTS.md）
├── system/                     # RTOS 抽象（当前仅 FreeRTOS 实现）
├── util/                       # 轻量工具层
├── test/                       # 测试入口 + emdevif_test_framework 子模块
├── support/                    # 平台适配层（stm32cubemx / esp-idf）
├── check_compile_features/     # 编译器 C++20 模块特性检测
├── cmake/                      # CMake 辅助脚本（utility.cmake）
├── docs/                       # 编码规范等文档
└── .agents/                    # 智能体技能（编码风格等）
```

### 嵌套子模块
`test/emdevif_test_framework` 是独立仓库（通过 `.gitmodules` 管理），提供纯 C 的单元测试框架，有独立 CI。改动测试入口或忽略列表机制需注意跨仓库兼容性。

## 编码规范
代码风格以 `docs/emdevif-coding-style.md` 为准，`.agents/skills/emdevif-coding-style/SKILL.md` 提供了 agent 执行时的检查清单。核心约定：

- 类型名 `PascalCase`，函数名 `camelCase`，变量/命名空间 `snake_case`
- 宏 `UPPER_SNAKE_CASE`，统一 `EMDEVIF_*` 前缀
- C 接口用 `.h` + `EMDEVIF_EXTERN_C` 宏，C++ 接口用 `.hpp` / `.cppm`
- 控制语句强制大括号，行宽 120，Google 风格基底的 4 空格缩进
- `#include` 最小化，禁止循环依赖

## 构建系统

### 关键 CMake 变量

| 变量 | 默认 | 说明 |
|---|---|---|
| `EMDEVIF_USE_CPP_MODULES` | `ON` | C++20 模块开关。构建前由 `check_compile_features/` 验证编译器支持 |
| `EMDEVIF_ENABLED_MODULES` | 全部 | 模块裁剪列表（分号分隔），如 `"logger;system;util"` |
| `EMDEVIF_USE_STM32CUBEMX` | `OFF` | 启用 STM32CubeMX 平台适配，与 `EMDEVIF_USE_ESP_IDF` 互斥 |
| `EMDEVIF_USE_ESP_IDF` | `OFF` | 启用 ESP-IDF 平台适配 |
| `EMDEVIF_ENABLE_TESTS` | `OFF` | 测试开关，开启后强制链接所有模块 |

### 平台适配
- `support/stm32cubemx/`：启用后引入 HAL 库和 CMSIS 启动文件
- `support/esp-idf/`：启用后适配 ESP-IDF 组件体系

两个选项互斥（`cmake/utility.cmake` 中的 `emdevif_assertMutuallyExclusiveOptions` 强制检查）。

## 总体开发原则
- `core` 为必选基础层，其他模块可按需裁剪。
- 保持"模块导入"和"头文件导入"两种方式接口语义一致。
- 需用户实现的底层能力统一放到 `emdevif_user_declares`，避免循环依赖。
- 不在 `emdevif` 主模块内引入强板级耦合实现。

## 子模块约束（合并说明）

### core
- API 稳定性优先，谨慎变更错误处理/断言/终止流程。
- 保持 C / C++ 边界清晰（`.h` 与 `.hpp`）。
- 不修改 `core/depends/` 下第三方代码。

### logger
- 与 `emdevif_user_declares::logger` 接口签名保持一致。
- 涉及锁/线程改动需兼容 `emdevif_system`。
- 避免用户实现反向链接本模块导致循环依赖。

### peripheral
- 保持模型层与实现层解耦。
- `peripheral_handle_map` 的键名语义要稳定。
- 审慎处理 `void*` 映射的类型安全。
- 详细设计、架构与扩展方式见 [peripheral/AGENTS.md](peripheral/AGENTS.md)。

### system
- 维持 RTOS 抽象边界，不向上泄漏平台细节。
- 非 FreeRTOS 场景保持 `emdevif_system_impl` 扩展点可用。
- 保证 ESP-IDF 与 STM32CubeMX 条件分支行为清晰。

### timeline
- 依赖用户提供单调时间源，不假设具体硬件计时器。
- 明确时间单位与溢出语义，避免 ms/us 混用。

### util
- 工具层保持轻量、可移植、低耦合。
- 基础模板工具改动需评估下游兼容性。

### test 与 emdevif_test_framework
- 测试入口命名与忽略列表机制保持兼容。
- 框架层保持纯 C、低依赖、低开销特性。
- 新测试优先覆盖公共接口与模块边界。

## 扩展指引

### 新增外设类型
emdevif 本身不直接操作外设，而是通过 `peripheral/` 子系统提供统一的 `Model → Impl` 解耦框架。要新增一种外设类型（如 I2C、ADC），请在 `peripheral/` 下按三层架构添加，详见 [peripheral/AGENTS.md](peripheral/AGENTS.md)。

### 新增功能模块（库级模块，非外设）
如需添加一个全新的库模块（如数学工具、数据压缩等），步骤如下：

1. 在仓库根目录创建模块目录（如 `compression/`），内建 `inc/` + `src/` + `CMakeLists.txt`
2. CMake 目标命名为 `emdevif_compression`
3. 在根 `CMakeLists.txt` 的 `emdevifAllModules` 列表中加入 `compression`
4. 在 AGENTS.md 中新增对应的子模块约束小节

## 验证要求
- 至少验证 `EMDEVIF_USE_CPP_MODULES=ON/OFF` 两条构建路径。
- 变更涉及 `system/logger/timeline` 时，验证用户声明接口路径可用。
- 变更涉及 `support/` 时，至少验证 STM32CubeMX 和 ESP-IDF 各一次构建。
- 修改公共头文件时，运行相关测试套件（test_suits 下对应目录）。

## 参考资料
- 编码规范：`docs/emdevif-coding-style.md`
- Agent 编码技能：`.agents/skills/emdevif-coding-style/SKILL.md`
- Peripheral 架构与扩展：`peripheral/AGENTS.md`
- 完整使用指南：`README.md`
