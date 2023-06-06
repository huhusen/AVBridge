//
// Created by uma on 2023/6/6.
//

#include "PluginRTMP.h"

Command PluginRTMP::React(std::any msg) {
    SPDLOG_INFO("PluginRTMP");
    return Start;
}
