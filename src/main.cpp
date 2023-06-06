#include <iostream>
//#include "env.h"
//#include "hv/TcpServer.h"
#include "EasyNvr.h"

//using namespace hv;
//using namespace std;

int main() {

    EasyNvr &nvr = EasyNvr::NewEasyNvr();
    nvr.Run();

//
//    TcpServer srv;
//    int rtmpPort = 1935;
//    int rtmpFd = srv.createsocket(rtmpPort);
//    if (rtmpFd < 0) {
//        SPDLOG_ERROR("RTMP Server Start Failed on {}.", rtmpPort);
//    }
//    SPDLOG_INFO("RTMP Server listen on port {} ", rtmpPort);
//    srv.onConnection = [](const SocketChannelPtr &channel) {
//        string peerAddr = channel->peeraddr();
//        if (channel->isConnected()) {
//            SPDLOG_INFO("{} connected! connfd={}", peerAddr.c_str(), channel->fd());
//        } else {
//            SPDLOG_INFO("{} disconnected! connfd={}", peerAddr.c_str(), channel->fd());
//        }
//    };
//    srv.onMessage = [](const SocketChannelPtr &channel, Buffer *buf) {
//        SPDLOG_INFO("len:{}",buf->size());
//        PRINT_HEX(buf->data(),buf->size());
//    };
//    srv.start();
//    while (getchar() != '\n');
    return 0;
}
