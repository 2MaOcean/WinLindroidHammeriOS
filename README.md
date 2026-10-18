# WinLindroidHammeriOS
这是一个何以谓的项目，目标是做出一个能用的，几乎兼容所有平台、所有程序的操作系统，依然许愿式编程。也许我有了Dsh，有百万亿token来挥霍时会捡起来做吧。

语言/运行时：原生支持 Java、Python、C++、C#、Rust、Go、Node.js；原生运行各应用，非转译层；可在 arm64 等架构上流畅运行其他架构应用。
优先级：P0 骨架 + 权限 + 可启动内核 + README + .gitignore + LICENSE；P1 命令行 + 构建 + CI + 视频脚本；P2 驱动 + FS + 网络 + 图形；P3 多架构原生运行 + 100 应用测试 + ISO；P4 SDK + 商店 + 社区。
合规：不违反 Apple/Google/微软/华为许可；不内置未授权闭源代码；声明兼容层与原生界限；遵守开源协议；出口与加密合规

> 目标：一个全新的自主操作系统项目。  
> 当前状态：**P0 最小可启动骨架**。这不是完整 OS，也不是套壳系统。

## 当前 P0 范围

P0 只完成以下内容：

- 可启动的 x86 Multiboot2 最小内核
- VGA 文本输出
- 五级权限模型骨架：Restricted / User / Admin / Root / SuperRoot
- Capability 细粒度权限位骨架
- SuperRoot / Restricted 隐藏命令入口策略骨架
- 内核热替换“仅 SuperRoot 可尝试，且必须离线密钥 + 物理确认 + 强制快照”的策略占位
- README、LICENSE、.gitignore、阶段文档

P0 **不包含**：

- Windows exe / macOS app / Linux deb / Android apk / 鸿蒙 / iOS 应用兼容
- Java/Python/C++/C#/Rust/Go/Node.js 原生运行时
- 多架构运行、驱动、文件系统、网络、GUI、商店、SDK
- 真实 SuperRoot U 盘密钥、真实内核热替换、真实安全启动链
- 任何“已通过 100 应用测试”的声明

## 构建依赖

Ubuntu/Debian 示例：

```bash
sudo apt update
sudo apt install build-essential gcc-multilib grub-pc-bin grub-common xorriso qemu-system-x86