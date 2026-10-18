# 权限模型 P0 骨架

| 等级 | GUI 入口 | 隐藏命令 | 内核热替换 |
|---|---|---|---|
| Restricted | 否 | `enter-restricted` | 禁止 |
| User | 是 | 否 | 禁止 |
| Admin | 是 | 否 | 禁止 |
| Root | 是（GUI 切换） | 否 | 禁止 |
| SuperRoot | 否 | `unlock-superroot` | 仅允许发起验证 |

## 已实现
- 五级枚举、GUI 可见性、隐藏命令策略、热替换发起策略、Capability 位骨架

## 未实现
- 真实 U 盘密钥、物理确认、强制快照、审计日志、沙盒、MAC、FDE/FBE、真实热替换

## 安全声明
P0 权限模型只是策略与接口骨架，不能提供真实安全保证。
