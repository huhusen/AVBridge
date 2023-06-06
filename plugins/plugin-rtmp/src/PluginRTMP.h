//
// Created by uma on 2023/6/6.
//

#ifndef EASYNVR_PLUGINRTMP_H
#define EASYNVR_PLUGINRTMP_H

#include "core/IPlugin.h"
#include "env.h"

class PluginRTMP : public IPlugin {
public:
    static PluginRTMP &NewPluginRTMP() {
        static PluginRTMP instance;  // 在首次调用时进行实例化，保证只有一个实例
        return instance;
    }

    Command React(std::any msg) override;

private:
    PluginRTMP() {}  // 构造函数私有化，防止外部实例化对象
    ~PluginRTMP() {} // 可以选择定义私有析构函数来确保对象无法通过 delete 销毁

    // 禁用拷贝构造函数和赋值运算符，以防止复制实例
    PluginRTMP(const PluginRTMP &) = delete;

    PluginRTMP &operator=(const PluginRTMP &) = delete;
};


#endif //EASYNVR_PLUGINRTMP_H
