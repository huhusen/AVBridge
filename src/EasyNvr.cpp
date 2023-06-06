//
// Created by uma on 2023/6/5.
//

#include "EasyNvr.h"
#include "spdlog/fmt/fmt.h"


void EasyNvr::Run() {

    PluginManager *plgMgr = new PluginManager();
    const char *pluginFile = "plugin-rtmp.dll";
    IPlugin *plugin = plgMgr->loadPlugin(pluginFile);
    if (plugin != nullptr) {
        SPDLOG_INFO("Loaded plugin ->{}<- successfully. Name: {}, Version: {}, Author: {}.", pluginFile,
                    Blue(plugin->Name),
                    plugin->Version, plugin->Author);
    }
    delete plgMgr;
}