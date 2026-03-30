# AGENTS.md

## 模块定位
`emdevif` 提供嵌入式开发通用接口抽象（core/logger/timeline/peripheral/system/util），并作为 `rmdev` 的底层基础能力。

## 总体开发原则
- `core` 为必选基础层，其他模块可按需裁剪。
- 保持“模块导入”和“头文件导入”两种方式接口语义一致。
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

## 验证要求
- 至少验证 `EMDEVIF_USE_CPP_MODULES=ON/OFF` 两条构建路径。
- 变更涉及 `system/logger/timeline` 时，验证用户声明接口路径可用。
