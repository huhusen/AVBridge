//
// Created by uma on 2023/6/6.
//

#ifndef EASYNVR_PLUGINRTMP_H
#define EASYNVR_PLUGINRTMP_H

#include "core/IPlugin.h"
#include "env.h"

class __declspec(dllexport) PluginRTMP : public IPlugin {
public:

    Command React(std::any msg) override;

    PluginRTMP(const char *name, const char *version, const char *author) {
        this->Name = name;
        this->Version = version;
        this->Author = author;
    }

    ~PluginRTMP() {};

};

extern "C" __declspec(dllexport) IPlugin *Install() {
    return new PluginRTMP("rtmp-plugin", "0.0.1", "EasyNvr");
}
#endif //EASYNVR_PLUGINRTMP_H
