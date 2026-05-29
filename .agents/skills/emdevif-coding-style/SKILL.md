---
name: emdevif-coding-style
description: 被指示遵循 emdevif-coding-style 时，应用工程约束并强制参照编码规范文档。
---

# emdevif coding style skill

## When to use

- 当你被指示遵循 `emdevif-coding-style` 时，新增、修改、重构代码时使用。
- 当你需要判断代码风格、命名一致性、模块边界是否合规时使用。

## Source of truth

代码风格与命名的规范主文档为：

- `emdevif/docs/emdevif-coding-style.md`

在涉及格式、命名、C/C++ 边界、C++20 模块时，必须以该文档为准。

## Additional agent constraints

以下约束是给智能体的执行规则，补充于用户规范文档之外：

### 1) 工程原则
- 可读性优先，避免不必要技巧化实现。
- 公共接口稳定优先，最小化破坏性变更。
- 平台解耦，避免把板级细节引入核心层。

### 2) 静态检查
- 不引入新的高置信 `.clang-tidy` 告警。
- 重点关注：`bugprone-*`、`cppcoreguidelines-*`、`modernize-*`、`performance-*`、`readability-*`。

### 3) 错误处理
- 返回显式错误，不静默失败。
- 断言用于开发期不变量，不替代运行时错误处理。

### 4) 依赖与边界
- include 最小化，避免公共头重依赖。
- 禁止循环依赖，优先前置声明。
- 用户实现入口经 `emdevif_user_declares` 注入。
- 禁止修改第三方代码：`core/depends/**`。

### 5) 测试与文档同步
- 公共接口变化应同步测试/示例/文档。
- 测试命名保持与忽略列表机制兼容。

## Execution checklist

- [ ] 已读取并遵循 `emdevif/docs/emdevif-coding-style.md`
- [ ] 未引入关键静态检查告警
- [ ] 未破坏模块边界与接口稳定性
- [ ] 必要测试与文档已同步
