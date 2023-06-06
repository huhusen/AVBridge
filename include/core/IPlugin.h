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
public:


    virtual Command React(std::any msg) = 0;

    virtual ~IPlugin() {}  // 声明为虚析构函数
public:
    std::string Name;
    std::string Version;
    std::string Author;
};

#endif //EASYNVR_PLUGIN_H
