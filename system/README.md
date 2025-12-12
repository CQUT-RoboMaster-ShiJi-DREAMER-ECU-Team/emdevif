# emdevif_system

操作系统 API 封装，用于实现系统操作。

此仓库提供 FreeRTOS、Linux（后续计划）的实现。对于其他的操作系统，将通过子仓库的方式提供。

## 依赖

`emdevif_util`

## 配置选项

| CMake 缓存变量                       | 类型     | 默认值        | 说明                                                                               |
|----------------------------------|--------|------------|----------------------------------------------------------------------------------|
| EMDEVIF_THREAD_SYSTEM            | String | "FreeRTOS" | 使用的操作系统的名称                                                                       |
| EMDEVIF_FREERTOS_USE_STM32CUBEMX | Bool   | OFF        | 是否使用 STM32CubeMX 生成的 FreeRTOS 库。如果要启用，需要确保变量 `EMDEVIF_USE_STM32CUBEMX` 被设置为 `ON` |
