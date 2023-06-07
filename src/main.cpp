
#include "EasyNvr.h"

//#include <string>
//#include "hv/TcpServer.h"
//#include "env.h"
//#include "iostream"
//using namespace hv;
int main() {
    EasyNvr *nvr = new EasyNvr();
    nvr->Run();

//    IPlugin::Tcp t;
//    TcpServer srv;
//    nvr->servTcp(t, &srv);

//
////
//    TcpServer srv;
//    int rtmpPort = 1935;
//    int rtmpFd = srv.createsocket(rtmpPort);
//    if (rtmpFd < 0) {
//        SPDLOG_ERROR("RTMP Server Start Failed on {}.", rtmpPort);
//    }
//    SPDLOG_INFO("RTMP Server listen on port {} ", rtmpPort);
//    srv.onConnection = [](const SocketChannelPtr &channel) {
//        std::string peerAddr = channel->peeraddr();
//        if (channel->isConnected()) {
//            SPDLOG_INFO("{} connected! connfd={}", peerAddr.c_str(), channel->fd());
//        } else {
//            SPDLOG_INFO("{} disconnected! connfd={}", peerAddr.c_str(), channel->fd());
//        }
//    };
//    srv.onMessage = [](const SocketChannelPtr &channel, Buffer *buf) {
//        SPDLOG_INFO("len:{}", buf->size());
//        PRINT_HEX(buf->data(), buf->size());
//    };
//    srv.start();
    while (getchar() != '\n') {};
    return 0;
}
