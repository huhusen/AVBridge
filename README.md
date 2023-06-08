# AVBridge 音视频流媒体转换与发布平台

本项目是一个功能强大的音视频流媒体转换与发布平台，提供插件化的功能扩展、HTTP 和 TCP 服务注册，并计划未来支持多种协议(RTSP/RTMP/HLS/HTTP-FLV/WebSocket-FLV/GB28181/HTTP-TS/WebSocket-TS/HTTP-fMP4/WebSocket-fMP4/MP4/WebRTC)同时支持协议互转。它可以用于实时转换、处理和发布音视频流，满足各种音视频处理需求。

## 功能特性

- 插件化的架构，方便扩展和定制功能。
- 提供 HTTP 和 TCP 服务注册，方便与外部系统进行交互和集成。
- 支持流媒体格式的转换，包括音频和视频的编码和解码。
- 实时流媒体的转发和发布，支持多种协议和传输方式。

## 插件开发
本平台支持插件开发，您可以根据需求开发自定义的功能插件。插件可以用于实现特定的音视频处理逻辑、协议支持等功能。

### 插件开发要求

- 插件必须实现特定的接口，并按照约定的方式进行注册和加载。
- 插件可以包含自定义的音视频处理逻辑，或实现特定的协议支持。

### 插件开发示例

以下是一个示例插件的代码结构，供您参考：

```cpp

#include "core/IPlugin.h"
#include "env.h"

class __declspec(dllexport) PluginRTMP : public IPlugin {
public:

    Command React(std::any msg) override;

    PluginRTMP(const char *name, const char *version, const char *author);

    PluginRTMP();

    ~PluginRTMP() {};
private:
    void HelloHttp();

    void TcpServer();
};

extern "C" __declspec(dllexport) IPlugin *Install() {
    return new PluginRTMP("rtmp-plugin", "0.0.1", "AVBridge");
}
```
# 使用指南
## 编译和运行
1. 首先，确保您已经安装了所需的依赖项和开发工具链。
2. 克隆项目到本地并进入项目目录。
3. 执行编译命令，并生成可执行文件。
4. 运行可执行文件，启动音视频流媒体转换与发布平台。 
## 配置文件
本项目提供了一个配置文件，用于配置平台的参数和插件加载。您可以根据需要进行相应的配置。

## 待开发功能
当前版本中还未实现对 RTMP 协议的支持。我们计划在未来的版本中加入对 RTMP 的支持，以满足更广泛的音视频流转换和发布需求。

## 反馈和贡献
如果您发现任何问题或有任何建议，请随时向我们提出。我们欢迎社区的贡献者，如果您有兴趣为项目做出贡献，请参阅 CONTRIBUTING.md 文件了解更多信息。

## 鸣谢
https://github.com/shank03/HMAC_SHA256
