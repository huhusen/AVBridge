// File: AVBridge.h
// Author: uma
// Date: 2023/6/5

#ifndef AVBridge_AVBridge_H
#define AVBridge_AVBridge_H

#include "PluginManager.h"

class AVBridge {
public:
    AVBridge() {}  // 构造函数私有化，防止外部实例化对象
    ~AVBridge();// 可以选择定义私有析构函数来确保对象无法通过 delete 销毁

    void Run();

    void ServeHttp();

    void ServeTcp(IPlugin *plugin, TcpServer *srv);

private:
    HttpService router;
    HttpServer server;
    std::map<std::string, TcpServer *> pluginTcpServers;
};

#endif // AVBridge_AVBridge_H
