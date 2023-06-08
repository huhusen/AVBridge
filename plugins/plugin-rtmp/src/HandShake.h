//
// Created by uma on 2023/6/8.
//

#ifndef AVBRIDGE_HANDLESHAKE_H
#define AVBRIDGE_HANDLESHAKE_H

#include <cstdint>
#include <vector>
#include "Exception.h"
#include "AVBridgeEnv.h"


class SimpleHandShake {
public:
    SimpleHandShake() {};

    RTMPException execute(std::vector<uint8_t> C1);

    ~SimpleHandShake() {};
};

class ComplexHandShake {
public:

    struct ClientSchemeInfo {
        int scheme;
        std::vector<uint8_t> challenge;
        std::vector<uint8_t> digest;
        bool ok;
        std::string error;
    };


    ComplexHandShake() {};

    RTMPException execute(std::vector<uint8_t> C1);

    ClientSchemeInfo validateClient(std::vector<uint8_t> C1);

    ClientSchemeInfo clientScheme(std::vector<uint8_t> C1, int scheme);

    int scheme0_Digest_Offset(std::vector<uint8_t> C1S1);

    int scheme0_Key_Offset(std::vector<uint8_t> C1S1);

    int scheme1_Digest_Offset(std::vector<uint8_t> C1S1);

    int scheme1_Key_Offset(std::vector<uint8_t> C1S1);

    ~ComplexHandShake() {};
};

class Handshake {
public:
    Handshake() {};

    ~Handshake() {};

    RTMPException execute(void *buf, size_t len);

    SimpleHandShake simpleHandShake;
    ComplexHandShake complexHandShake;
};

#endif //AVBRIDGE_HANDLESHAKE_H
