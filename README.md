# TPKit

Transport Kit

## Contents

- [Bazel](./Bazel) - Bazel 配置依赖
- [CMake](./CMake) - CMake 配置依赖
- [Examples](./Examples) - 使用示例
- [Scripts](./Scripts) - 构建相关的脚本
- [Source](./Source)
  - [Transport](./Source/Transport) - 数据传输模块（提供上传下载优化加速）
  - [TLV](./Source/TLV) - 数据序列化协议
  - [P2P](./Source/P2P) - P2P 通信框架
  - [Tun](./Source/Tun) - 网络层代理模块
  - [Debugger](./Source/Debugger) -  本地调试模块
  - [Telemetry](Source/Telemetry) - 遥测模块
  - [NetProbe](./Source/NetProbe) - 网络拨测模块
  - [JSR](./Source/JSR) - 轻量 JS 运行时，用于实现插件系统
  - [TX](./Source/TX) - 基础运行时库  (**T**emplate e**X**tended)
  - [ThirdParty](./Source/ThirdParty) - 第三方库
- [Platform](./Source/Platform) - 各个平台对 TPKit C API 的绑定
- [Tests](./Tests) - 功能测试
