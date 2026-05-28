# OBS Bilibili 直播插件（二开）

这是一个基于 [`Zarosmm/obs-bilibili-stream`](https://github.com/Zarosmm/obs-bilibili-stream) 的二开版本，用于简化在 Bilibili 平台上的 OBS 直播流程。

插件支持扫码登录 Bilibili、更新直播间信息、开启/关闭直播，并在开启直播后自动把 Bilibili 返回的 RTMP 地址和推流码写入 OBS 直播设置。若自动写入失败，插件也会把 RTMP 地址和推流码分开显示，并提供单独复制按钮。

**[English README](README_en.md)**

## 安装方法

### Windows

推荐使用 `.exe` 安装器：

1.  **下载插件**：从 [Releases 页面](https://github.com/Chil1T/obs-bilibili-stream/releases) 下载最新的 `bilibili-stream-for-obs-*-windows-x64-installer.exe`。
2.  **关闭 OBS**：安装前退出 OBS Studio。
3.  **运行安装器**：双击 `.exe`，按提示安装。插件会安装到 `C:\ProgramData\obs-studio\plugins`。
4.  **启动 OBS**：重新启动 OBS Studio，插件将自动加载。

也可以手动安装 `.zip`：

1.  **下载插件**：从 [Releases 页面](https://github.com/Chil1T/obs-bilibili-stream/releases) 下载最新的 `bilibili-stream-for-obs-*-windows-x64.zip`。
2.  **解压文件**：将压缩包解压。
3.  **放置目录**：将解压后的文件夹移动至以下路径：
    `C:\ProgramData\obs-studio\plugins`
4.  **校验结构**：请确保您的目录结构严格遵守以下格式：
    ```text
    C:\ProgramData\obs-studio\plugins\
    └── bilibili-stream-for-obs\
        ├── bin\
        │   └── 64bit\
        │       └── bilibili-stream-for-obs.dll
        └── data\
            └── locale\
                └── (相关的 .ini 语言文件)
    ```
5.  **启动 OBS**：重新启动 OBS Studio，插件将自动加载。

<div align="center">
  <img width="785" height="916" alt="image" src="https://github.com/user-attachments/assets/bf0b35cb-b7ce-49d5-b783-41cbaffefd08" />
  <p><i>OBS 官方插件安装路径说明</i></p>
</div>

### macOS

1.  **下载插件**：从 [Releases 页面](https://github.com/Zarosmm/obs-bilibili-stream/releases) 下载最新的 `bilibili-stream-for-obs-*-macos-universal.pkg`。
2.  **安装插件**：双击 `.pkg` 文件，按提示完成安装。
3.  **启动 OBS**：重新启动 OBS Studio，插件将自动加载。

插件将安装到 `/Library/Application Support/obs-studio/plugins/` 目录。

### Ubuntu/Debian

1.  **下载插件**：从 [Releases 页面](https://github.com/Zarosmm/obs-bilibili-stream/releases) 下载最新的 `.deb` 包（例如 `bilibili-stream-for-obs-*-x86_64-ubuntu-22.04.deb`）。
2.  **安装插件**：运行以下命令：
    ```bash
    sudo dpkg -i bilibili-stream-for-obs-*.deb
    ```
3.  **修复依赖**（如需要）：
    ```bash
    sudo apt-get install -f
    ```
4.  **启动 OBS**：重新启动 OBS Studio，插件将自动加载。

插件将安装到 `/usr/lib/obs-plugins/` 目录。

## 使用方法

1. **登录 Bilibili**：
    - 打开 OBS Studio，导航到菜单栏的 **Bilibili直播** → **登录**。
    - 选择 **扫码登录**（使用手机扫描二维码）。
    - 登录成功后，菜单中的"登录状态"将显示为"已登录"。

2. **更新直播间信息**：
    - 导航到 **Bilibili直播** → **更新直播间信息**。
    - 输入直播间标题，选择直播分区和子分区（例如"网游" → "英雄联盟"）。
    - 点击"确认"保存设置。

3. **开始直播**：
    - 导航到 **Bilibili直播** → **开始直播**。
    - 插件会尝试自动把 **RTMP 地址** 和 **推流码** 写入 OBS 的直播设置。
    - 若自动写入成功，确认弹窗后点击 OBS 界面右下角的 **开始直播** 按钮。
    - 若自动写入失败，弹窗会分开显示 RTMP 地址和推流码，可使用单独复制按钮手动填入 OBS。

4. **结束直播**：
    - 在 OBS 界面右下角点击 **停止直播**。
    - 返回到 **Bilibili直播** → **停止直播**，以关闭 Bilibili 直播间。

## 注意事项

- **日志查看**：如果遇到问题，检查 OBS 日志文件：
  - Windows: `C:\Users\<YourUser>\AppData\Roaming\obs-studio\logs`
  - macOS: `~/Library/Logs/obs-studio/`
  - Linux: `~/.config/obs-studio/logs/`

## 依赖

- OBS Studio 30.0 或更高版本
- Windows / macOS / Ubuntu 22.04 或更高版本

## 二开说明

本仓库保留原项目 GPL 授权和上游归属。当前二开重点是：

- 自动写入 OBS 自定义 RTMP 直播设置。
- 优化推流信息弹窗，支持 RTMP 地址和推流码单独复制。
- 为 Windows Release 增加自动安装器。

详细计划见 [docs/fork-plan.md](docs/fork-plan.md)。

## 贡献

欢迎提交 issue 或 pull request。若改动涉及上游原始逻辑，请尽量保持补丁小而清晰，方便后续同步上游。

## Star History

[![Star History Chart](https://api.star-history.com/svg?repos=Zarosmm/obs-bilibili-stream&type=date&legend=top-left)](https://www.star-history.com/#Zarosmm/obs-bilibili-stream&type=date&legend=top-left)
