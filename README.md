# TPKit

## Contents

- [Bazel](./Bazel) - Bazel 配置依赖
- [CMake](./CMake) - CMake 配置依赖
- [Examples](./Examples) - 使用示例
- [Scripts](./Scripts) - 构建相关的脚本
- [Source](./Source) - 核心模块们
  - [API](./Source/API) - 核心模块的 C API
  - [Platform](./Source/Platform) - 各个平台对核心模块 API 的绑定
  - [Transport](./Source/Transport) - 数据传输模块
  - [TLV](./Source/TLV) - 通用二进制网络通信协议模块
  - [Tun](./Source/Tun) - 网络层代理模块
  - [TX](./Source/TX) - 基础运行时库  (**T**emplate e**X**tended)
  - [ThirdParty](./Source/ThirdParty) - 核心模块们共同依赖的第三方库
  - [P2P](./Source/P2P) - 自研 P2P 通信模块
  - [JSR](./Source/JSR) - 轻量 JS 运行时，用于实现插件系统
- [Tests](./Tests) - 功能测试
