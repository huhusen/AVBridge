//
// Created by uma on 2023/6/6.
//

#ifndef AVBridge_PLUGINRTMP_H
#define AVBridge_PLUGINRTMP_H

#include "core/IPlugin.h"
#include <set>
#include "HandShake.h"


class __declspec(dllexport) PluginRTMP : public IPlugin {
public:

    Command React(std::any msg) override;

    PluginRTMP(const char *name, const char *version, const char *author);

    PluginRTMP();

    ~PluginRTMP() {};
private:
    void HelloHttp();

    void TcpServer();


private:
    std::set<std::string> clients;
    Handshake handshake;
};
extern "C" __declspec(dllexport) IPlugin *Install() {
    return new PluginRTMP("rtmp-plugin", "0.0.1", "AVBridge");
}
#endif //AVBridge_PLUGINRTMP_H
