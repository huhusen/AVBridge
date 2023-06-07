// File: EasyNvr.h
// Author: uma
// Date: 2023/6/5

#ifndef EASYNVR_EASYNVR_H
#define EASYNVR_EASYNVR_H

#include "PluginManager.h"

class EasyNvr {
public:
    EasyNvr() {}  // 构造函数私有化，防止外部实例化对象
    ~EasyNvr() {
        SPDLOG_INFO("123");
    } // 可以选择定义私有析构函数来确保对象无法通过 delete 销毁

    void Run();

    void ServeHttp();

    void ServeTcp(IPlugin *plugin, TcpServer *srv);

private:
    HttpService router;
    HttpServer server;
    std::map<std::string, TcpServer *> pluginTcpServers;
};

#endif // EASYNVR_EASYNVR_H
