//
// Created by uma on 2023/6/12.
//

#ifndef AVBRIDGE_MSG_H
#define AVBRIDGE_MSG_H


#include "AVBridgeEnv.h"
#include <cstdint>

#include <vector>
#include "RTMP.h"


class RtmpMessage {
public:
    virtual std::vector<uint8_t> Encode() = 0;

    // Define other virtual functions or pure virtual functions as needed
};

struct Chunk {
    ChunkHeader Header;
    std::vector<uint8_t> Body;
    std::unique_ptr<RtmpMessage> MsgData;
};

class Uint32Message : public RtmpMessage {
public:
    explicit Uint32Message(uint32_t m) : m(m) {}

    std::vector<uint8_t> Encode() override {
        std::vector<uint8_t> b(4);
        uint32_t value = static_cast<uint32_t>(m);
        b[0] = static_cast<uint8_t>((value >> 24) & 0xFF);
        b[1] = static_cast<uint8_t>((value >> 16) & 0xFF);
        b[2] = static_cast<uint8_t>((value >> 8) & 0xFF);
        b[3] = static_cast<uint8_t>(value & 0xFF);
        return b;
    }

private:
    uint32_t m;
};




static
void GetRtmpMessage(Chunk *chunk) {
    ByteBuffer body(chunk->Body);
    switch (chunk->Header.messageHeader.messageTypeID) {
        case RTMP_MSG_CHUNK_SIZE:
        case RTMP_MSG_ABORT:
        case RTMP_MSG_ACK:
        case RTMP_MSG_ACK_SIZE: {
            if (body.getSize() < 4) {
                SPDLOG_ERROR("chunk.Body < 4");
                return;
            }
            std::unique_ptr<RtmpMessage> ptr(new Uint32Message(body.getInt()));
            chunk->MsgData = std::move(ptr);
            break;
        }
        case RTMP_MSG_USER_CONTROL: {

            break;
        }
        case RTMP_MSG_BANDWIDTH: {

            break;
        }
        case RTMP_MSG_EDGE:
        case RTMP_MSG_AUDIO:
        case RTMP_MSG_VIDEO:
        case RTMP_MSG_AMF3_METADATA:
        case RTMP_MSG_AMF3_SHARED:
        case RTMP_MSG_AMF3_COMMAND:

            break;
        case RTMP_MSG_AMF0_METADATA:
        case RTMP_MSG_AMF0_SHARED:
        case RTMP_MSG_AMF0_COMMAND:

            break;
        case RTMP_MSG_AGGREGATE:
            break;
        default:
            break;
    }
};

#endif //AVBRIDGE_MSG_H
