//
// Created by uma on 2023/6/5.
//

#ifndef EASYNVR_PLUGIN_H
#define EASYNVR_PLUGIN_H

#include "hv/HttpServer.h"
#include "hv/TcpServer.h"
#include <iostream>
#include <any>

using namespace hv;

enum Command {
    Start,     // 0
    Pause,    // 1
    Stop,     // 2
};

typedef std::function<int(HttpRequest *, HttpResponse *)> HttpFunc;
typedef std::map<std::string, HttpFunc> HTTPMethod;

typedef std::function<void(const SocketChannelPtr &)> TcpOnConnection;

typedef std::function<void(const SocketChannelPtr &, Buffer *)> TcpOnMessage;


class IPlugin {
public:


    virtual Command React(std::any msg) = 0;


    virtual ~IPlugin() {}  // 声明为虚析构函数
public:
    struct Tcp {
        TcpOnConnection onConnection;
        TcpOnMessage onMessage;
        int port;
    };
    std::string Name = "Name";
    std::string Version = "Version";
    std::string Author = "Author";
    HTTPMethod PluginHTTPMethod;
    Tcp TcpServ;

};

#endif //EASYNVR_PLUGIN_H
