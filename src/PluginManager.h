// File: PluginManager.h
// Author: uma
// Date: 2023/6/6

#ifndef AVBridge_PLUGINMANAGER_H
#define AVBridge_PLUGINMANAGER_H

#ifdef _WIN32
// #include <windows.h>
#else
#include <dlfcn.h>
#endif

#include <map>
#include <string>

#include "core/IPlugin.h"
#include "AVBridgeEnv.h"

class PluginManager {
public:
    PluginManager() {}

    ~PluginManager() {
        UnloadAllPlugins();
    }

    IPlugin *LoadPlugin(const std::string &pluginFileName);

    void UnloadPlugin(const std::string &pluginName);

    IPlugin *GetPluginByName(const std::string &pluginName);

private:
    struct PluginData {
        void *pluginHandle;
        IPlugin *plugin;
        std::string pluginFileName;
    };

    std::map<std::string, PluginData> plugins;

    void UnloadAllPlugins() {
        for (const auto &pluginData: plugins) {
            if (pluginData.second.plugin != nullptr) {
                delete pluginData.second.plugin;
            }
            // Ð¶ÔØ²å¼þ
#ifdef _WIN32
            FreeLibrary(static_cast<HMODULE>(pluginData.second.pluginHandle));
#else
            dlclose(pluginData.second.pluginHandle);
#endif
        }
    }
};

#endif // AVBridge_PLUGINMANAGER_H
