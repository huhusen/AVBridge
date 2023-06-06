//
// Created by uma on 2023/6/5.
//

#ifndef EASYNVR_EASYNVR_H
#define EASYNVR_EASYNVR_H

#include "core/IPlugin.h"
#include "env.h"
#include "PluginManager.h"


class EasyNvr {
public:
    static EasyNvr &NewEasyNvr() {
        static EasyNvr instance;  // 在首次调用时进行实例化，保证只有一个实例

        return instance;
    }

    void Run();

private:
    EasyNvr() {}  // 构造函数私有化，防止外部实例化对象
    ~EasyNvr() {} // 可以选择定义私有析构函数来确保对象无法通过 delete 销毁

    // 禁用拷贝构造函数和赋值运算符，以防止复制实例
    EasyNvr(const EasyNvr &) = delete;

    EasyNvr &operator=(const EasyNvr &) = delete;
};


#endif //EASYNVR_EASYNVR_H
