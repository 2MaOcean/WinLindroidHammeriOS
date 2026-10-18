# WinLindroidHammeriOS

> 目标：一个全新的自主操作系统项目。  
> 当前状态：**P1 最小交互骨架**。这不是完整 OS，也不是套壳系统。

## 当前进展 (P1 阶段)

P1 阶段我们完成了：
- **纯串口控制台**：彻底放弃传统的 VGA 显存（`0xB8000`），改用串口输出。完美兼容 UTM / QEMU 等虚拟机，不再死机。
- **输入输出交互**：支持退格、回车，解决双输入乱码问题。
- **交互式 Shell (wlhios)**：内建基础命令 `help`、`whoami`、`clear`。
- **权限系统实战**：默认 `User` 级别，尝试执行 `unlock-superroot` 会被安全拦截。
- **CI 多分支构建**：GitHub Actions 会自动为所有分支打包 ISO。

P0 阶段完成的基础内容：
- i386 Multiboot2 可启动内核
- 五级权限模型骨架（Restricted / User / Admin / Root / SuperRoot）
- SuperRoot / Restricted 隐藏命令策略骨架

## 当前不支持

- 图形界面（GUI）、文件系统、网络、多架构原生运行
- Windows exe / macOS app / Linux deb / Android apk 等应用兼容层
- 真实的内核热替换、U盘物理密钥验证

## 如何运行

1. 在仓库的 [Releases](https://github.com/2MaOcean/WinLindroidHammeriOS/releases) 页面下载最新的 `wlhios.iso`。
2. 使用 UTM、VirtualBox 或 QEMU 等 x86 虚拟机挂载启动。
3. **重要**：由于系统默认使用串口输出，如果你在 UTM 中运行，请打开**串口终端（Serial）**界面，才能看到画面和输入命令。

## 路线图

- P0：骨架 + 权限 + 可启动内核 ✅
- P1：纯串口交互 + 命令行基础 ✅
- P2：物理内存管理 + 中断时钟 + 文件系统骨架（规划中）
- P3：多架构原生运行研究 + 100 应用测试框架
- P4：开发者 SDK + 商店与社区生态
- P5：多媒体与图形界面完善

## 安全声明

本项目当前处于极早期的技术验证阶段，权限模型仅为策略骨架，无真实隔离与认证能力。请勿用于生产环境。

## License

MIT，见 `LICENSE`。
