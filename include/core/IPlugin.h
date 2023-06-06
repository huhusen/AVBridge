//
// Created by uma on 2023/6/5.
//

#ifndef EASYNVR_PLUGIN_H
#define EASYNVR_PLUGIN_H

#include <iostream>
#include <any>

enum Command {
    Start,     // 0
    Pause,    // 1
    Stop,     // 2
};

class IPlugin {
    virtual Command React(std::any msg) = 0;
};

#endif //EASYNVR_PLUGIN_H
