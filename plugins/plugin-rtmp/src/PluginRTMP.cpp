//
// Created by uma on 2023/6/6.
//

#include "PluginRTMP.h"


Command PluginRTMP::React(std::any msg) {
    SPDLOG_INFO("PluginRTMPÖÐÎÄ");
    return Start;
}

PluginRTMP::PluginRTMP() {}


PluginRTMP::PluginRTMP(const char *name, const char *version, const char *author) {
    this->Name = name;
    this->Version = version;
    this->Author = author;
    this->HelloHttp();
    this->TcpServer();
}

void PluginRTMP::HelloHttp() {
    this->PluginHTTPMethod["hello"] = [this](HttpRequest *req, HttpResponse *resp) {
        return resp->String(this->Name);
    };
}

void PluginRTMP::TcpServer() {
    this->TcpServ.onMessage = [this](const SocketChannelPtr &channel, Buffer *buf) {

        SPDLOG_INFO("len:{}", buf->size());
        PRINT_HEX(buf->data(), buf->size());
        if (clients.count(channel->peeraddr()) == 0) {
            SPDLOG_INFO("New Rtmp Connection");
            this->handshake.execute(channel, buf);
            clients.insert(channel->peeraddr());
        } else {


        }

    };
    this->TcpServ.onConnection = [](const SocketChannelPtr &channel) {
        std::string peerAddr = channel->peeraddr();
        if (channel->isConnected()) {
            SPDLOG_INFO("{} connected! connfd={}", peerAddr.c_str(), channel->fd());
        } else {
            SPDLOG_INFO("{} disconnected! connfd={}", peerAddr.c_str(), channel->fd());
        }
    };
    this->TcpServ.port = 1936;
}

