//
// Created by uma on 2023/6/8.
//

#ifndef AVBRIDGE_HANDLESHAKE_H
#define AVBRIDGE_HANDLESHAKE_H

#include <cstdint>
#include <vector>
#include "Exception.h"
#include "AVBridgeEnv.h"
#include "hv/TcpServer.h"

class SimpleHandShake {
public:
    SimpleHandShake() {};

    RTMPException execute(ByteBuffer C1, const hv::SocketChannelPtr &channel);

    ~SimpleHandShake() {};
};

class ComplexHandShake {
public:

    struct ClientSchemeInfo {
        int scheme;
        std::vector<uint8_t> challenge;
        std::vector<uint8_t> digest;
        bool ok;
    };


    ComplexHandShake() {};

    RTMPException execute(ByteBuffer C1, const hv::SocketChannelPtr &channel);

    ClientSchemeInfo validateClient(ByteBuffer C1);

    ClientSchemeInfo clientScheme(ByteBuffer C1, int scheme);

    int scheme0_Digest_Offset(std::vector<uint8_t> C1S1);

    int scheme0_Key_Offset(std::vector<uint8_t> C1S1);

    int scheme1_Digest_Offset(std::vector<uint8_t> C1S1);

    int scheme1_Key_Offset(std::vector<uint8_t> C1S1);

    std::vector<uint8_t> create_S1();

    std::vector<uint8_t> create_S2();

    int scheme_Digest_Offset(const std::vector<uint8_t> &C1S1, int scheme);


    ~ComplexHandShake() {};
};

class Handshake {
public:
    Handshake() {};

    ~Handshake() {};

    RTMPException execute(const hv::SocketChannelPtr &channel, hv::Buffer *buf);

    SimpleHandShake simpleHandShake;
    ComplexHandShake complexHandShake;
};

#endif //AVBRIDGE_HANDLESHAKE_H
