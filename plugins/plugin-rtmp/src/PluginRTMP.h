//
// Created by uma on 2023/6/6.
//

#ifndef AVBridge_PLUGINRTMP_H
#define AVBridge_PLUGINRTMP_H

#include "core/IPlugin.h"
#include "env.h"
#include <set>
#include <shared_mutex>
#include "Exception.h"


class __declspec(dllexport) PluginRTMP : public IPlugin {
public:

    Command React(std::any msg) override;

    PluginRTMP(const char *name, const char *version, const char *author);

    PluginRTMP();

    ~PluginRTMP() {};
private:
    void HelloHttp();

    void TcpServer();

    RTMPException *Handshake(void *buf, size_t len);

private:
    std::set<std::string> clients;
    std::shared_mutex mtx;
};

#endif //AVBridge_PLUGINRTMP_H
