//
// Created by uma on 2023/6/6.
//

#ifndef EASYNVR_PLUGINMANAGER_H
#define EASYNVR_PLUGINMANAGER_H


#ifdef _WIN32

#include <windows.h>

#else
#include <dlfcn.h>
#endif

#include "core/IPlugin.h"
#include "env.h"

class PluginManager {
public:
    PluginManager() {}

    ~PluginManager() {
        unloadAllPlugins();
    }


    IPlugin *loadPlugin(const std::string &pluginFileName);

    void unloadPlugin(const std::string &pluginName);

    IPlugin *getPluginByName(const std::string &pluginName);

private:

//    PluginManager() : plugins() {}



    // 禁用拷贝构造函数和赋值运算符，以防止复制实例



    struct PluginData {
        void *pluginHandle;
        IPlugin *plugin;
        std::string pluginFileName;
    };

    std::map<std::string, PluginData> plugins;

    void unloadAllPlugins() {
        for (const auto &pluginData: plugins) {
            if (pluginData.second.plugin != nullptr) {
                delete pluginData.second.plugin;
            }

            // 卸载插件
#ifdef _WIN32
            FreeLibrary(static_cast<HMODULE>(pluginData.second.pluginHandle));
#else
            dlclose(pluginData.second.pluginHandle);
#endif
        }
    }
};


#endif //EASYNVR_PLUGINMANAGER_H
