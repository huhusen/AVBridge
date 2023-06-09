//
// Created by uma on 2023/6/7.
//

#ifndef AVBRIDGE_EXCEPTION_H
#define AVBRIDGE_EXCEPTION_H

#include <iostream>
#include <exception>

class RTMPException {
public:
    RTMPException(const char *message) : errorMessage(message) {}

    std::string errorMessage;

};

#endif //AVBRIDGE_EXCEPTION_H
