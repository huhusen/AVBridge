//
// Created by uma on 2023/6/5.
//

#include "EasyNvr.h"

void EasyNvr::Run() {
    PluginRTMP &rtmp = PluginRTMP::NewPluginRTMP();
    rtmp.React("123");
}