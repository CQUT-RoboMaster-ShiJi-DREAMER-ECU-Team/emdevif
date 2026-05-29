# emdevif 编码规范

本文档面向使用者，仅说明编写/阅读 `emdevif` 代码时应遵守的具体编码规则。

## 1. 代码格式规范（强制）

项目格式以仓库 `.clang-format` 为准，核心约束如下：

- 基础风格：`BasedOnStyle: Google`
- 缩进：4 空格，不使用 Tab
- 行宽：`ColumnLimit: 120`
- 指针/引用：左对齐（`int* p`, `int& r`）
- include：不自动排序（`SortIncludes: Never`）
- case 标签不额外缩进（`IndentCaseLabels: false`）

### 1.1 大括号与换行

- 类、函数定义的大括号单独换行，其余的（struct、控制语句、命名空间的大括号）不换行。
- 控制语句（`if/for/while/do-while`）必须添加大括号，哪怕只有一行。

### 1.2 空格与对齐

- 二元运算符两侧保留空格。
- 逗号后空格。
- 尾注释与代码之间保留至少 2 个空格。

### 1.3 注释规范

- 注释要解释“为什么”，而非重复“代码在做什么”。
- 公共接口、非直观行为、约束条件必须有注释。
- 禁止保留失效注释、注释掉的大段旧代码。

## 2. 命名规范

本项目同时包含模块名、文件名、C API、C++ API，需分层遵循一致约定。

### 2.1 文件与目录命名

- 目录名：`snake_case`（如 `peripheral_handle_map`）
- 头文件名：`snake_case.hpp` / `snake_case.h`
- C++ 模块文件名：`snake_case.cppm`
- 如果存在遗留的不规范命名（如 `ErrorHandler.hpp`），新文件应遵循规范，旧文件可保留但不再新增同目录不规范命名。

### 2.2 CMake 目标命名

- `emdevif` 子模块目标统一前缀：`emdevif_`
- 示例：`emdevif_core`、`emdevif_logger`、`emdevif_system`
- 内部实现目标命名以下划线结尾，如 `emdevif_peripheral_impl_`。

### 2.3 命名空间

- 顶层命名空间统一为：`emdevif`
- 命名风格采用 `snake_case`，避免与类型/函数命名混淆。
- 用户注入实现命名空间固定：`emdevif::user_declares`
- 模块内子命名空间使用语义化小写（如 `logger`, `timeline`）

### 2.4 类型、函数、变量、常量

- 类型名（类/结构体/枚举）：`PascalCase`
  - 示例：`ErrorCode`, `InitGuard`, `SerialModel`
- 函数名：`camelCase`
  - 示例：`registerFatalHandler`, `getMicroseconds`
- 变量名：`snake_case`
  - 示例：`peripheral_handle_map`, `message_queue`
- 常量：
  - 宏常量：`UPPER_SNAKE_CASE`
  - `constexpr` 常量视作变量，使用 `snake_case`。

### 2.5 宏命名

- 宏名必须全大写+下划线，且带模块前缀避免污染：
  - `EMDEVIF_*`
  - `EMDEVIF_TEST_*`
- 宏参数必须加括号，避免副作用与优先级问题。

## 3. C 与 C++ 边界规范

- `.h`：优先保证 C/C++ 兼容。
- `.hpp` / `.cppm`：C++ 专用。
- 对外暴露 C 接口时：
  - 在 C++ 下使用 `EMDEVIF_EXTERN_C`/`EMDEVIF_EXTERN_C_BEGIN`/`EMDEVIF_EXTERN_C_END` 宏封装。
  - 头文件防重包含宏与 `#pragma once` 保持项目既有风格一致。

## 4. C++20 模块规范

- 模块名与头文件路径语义保持一致（如 `emdevif.core.error_handler` 对应 `emdevif/core/error_handler.hpp`）。
- 模块接口单元应只导出必要符号，避免泄露实现细节。
- 不在模块接口中引入不必要的大型依赖。
- 避免同一接口被“模块导入 + 头文件包含”混用导致 ODR 或编译问题。
