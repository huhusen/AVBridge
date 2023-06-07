// File: EasyNvr.cpp
// Author: uma
// Date: 2023/6/5

#include "AVBridge.h"

AVBridge::~AVBridge() {
    this->server.stop();
    for (const auto &tcpServer: this->pluginTcpServers) {
        tcpServer.second->stop();
    }
    delete pluginManager;
}

void AVBridge::Run() {
    this->ServeHttp();
    this->pluginManager = new PluginManager();
    const char *pluginFile = "plugin-rtmp.dll";
    IPlugin *plugin = this->pluginManager->LoadPlugin(pluginFile);
    if (plugin != nullptr) {
        SPDLOG_INFO("Loaded plugin '{}' successfully. Name: {}, Version: {}, Author: {}", Blue(pluginFile),
                    Blue(plugin->Name), plugin->Version, plugin->Author);

        if (!plugin->PluginHTTPMethod.empty()) {
            for (auto &httpMethod: plugin->PluginHTTPMethod) {
                const std::string &path = fmt::format("/{}/{}", plugin->Name, httpMethod.first);
                SPDLOG_INFO("A new route '{}' has been added by '{}'.", Green(path), Green(plugin->Name));
                this->router.Any(path.c_str(), httpMethod.second);
            }
        }
        if (plugin->TcpServ.onConnection && plugin->TcpServ.onMessage) {
            TcpServer *srv = new TcpServer();
            this->ServeTcp(plugin, srv);
            this->pluginTcpServers[plugin->Name] = srv;
        }
    }


}

void AVBridge::ServeHttp() {

    this->server.registerHttpService(&this->router);
    this->server.setPort(8080);
    this->server.setThreadNum(1);
    int ret = this->server.start();
    if (ret == 0) {
        SPDLOG_INFO(Blue("HttpServer started on port {}."), 8080);
    } else {
        SPDLOG_ERROR(Red("Failed to start the HttpServer on port {}."), 8080);
        exit(500);
    }
}

void AVBridge::ServeTcp(IPlugin *plugin, TcpServer *srv) {
    int fd = srv->createsocket(plugin->TcpServ.port);
    if (fd < 0) {
        SPDLOG_ERROR(Red("Failed to start the {} TcpServer on port {}."), plugin->Name, plugin->TcpServ.port);
    }
    SPDLOG_INFO(Blue("{} TcpServer started on port {}."), plugin->Name, plugin->TcpServ.port);
    srv->onConnection = plugin->TcpServ.onConnection;
    srv->onMessage = plugin->TcpServ.onMessage;
    srv->setThreadNum(4);
    srv->start();
    return;
}
