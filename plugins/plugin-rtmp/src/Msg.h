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

class UserControlMessage : public RtmpMessage {
public:
    explicit UserControlMessage() {}

    UserControlMessage(uint16_t eventType, const std::vector<std::uint8_t> &eventData) : EventType(eventType),
                                                                                         EventData(eventData) {}

    std::vector<uint8_t> Encode() override {
        std::vector<unsigned char> b;
        for (size_t i = 0, n = 2; i < n; i++) {
            b[i] = static_cast<unsigned char>(EventType >> ((n - i - 1) << 3));
        }
        return b;
    }

public:
    std::uint16_t EventType;
    std::vector<std::uint8_t> EventData;
};

class StreamIDMessage : public RtmpMessage {
public:
    explicit StreamIDMessage() {}

    StreamIDMessage(const UserControlMessage &userControlMessage, uint32_t streamId) : userControlMessage(
            userControlMessage), StreamID(streamId) {}

    std::vector<uint8_t> Encode() override {
        ByteBuffer bf(6);
        bf.putShort(userControlMessage.EventType);
        bf.putInt(StreamID);
        userControlMessage.EventData = bf.getRange(2, bf.getPosition());
        return bf.getBuffer();
    }

public:
    UserControlMessage userControlMessage;
    std::uint32_t StreamID;
};

class SetBufferMessage : public RtmpMessage {
public:
    explicit SetBufferMessage() {}

    SetBufferMessage(const StreamIDMessage &streamIdMessage, uint32_t millisecond) : streamIdMessage(streamIdMessage),
                                                                                     Millisecond(millisecond) {}

    std::vector<uint8_t> Encode() override {
        ByteBuffer bf(10);
        bf.putShort(streamIdMessage.userControlMessage.EventType);
        bf.putInt(streamIdMessage.StreamID);
        bf.putInt(Millisecond);
        streamIdMessage.userControlMessage.EventData = bf.getRange(2, bf.getPosition());
        return bf.getBuffer();
    }

public:
    StreamIDMessage streamIdMessage;
    std::uint32_t Millisecond;
};

class PingRequestMessage : public RtmpMessage {
public:
    explicit PingRequestMessage() {}

    PingRequestMessage(const UserControlMessage &userControlMessage, uint32_t timestamp) : userControlMessage(
            userControlMessage), Timestamp(timestamp) {}

    std::vector<uint8_t> Encode() override {
        ByteBuffer bf(6);
        bf.putShort(userControlMessage.EventType);
        bf.putInt(Timestamp);
        userControlMessage.EventData = bf.getRange(2, bf.getPosition());
        return bf.getBuffer();
    }

public:
    UserControlMessage userControlMessage;
    std::uint32_t Timestamp;
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
            if (body.getSize() < 4) {
                SPDLOG_ERROR("UserControlMessage.Body < 2");
                return;
            }
            UserControlMessage base(body.getInt(), body.getBuffer());

            switch (base.EventType) {
                case RTMP_USER_STREAM_BEGIN: {
                    std::unique_ptr<StreamIDMessage> m(new StreamIDMessage(base, 0));
                    if (base.EventData.size() >= 4) {
                        m->StreamID = body.getInt();
                    }
                    chunk->MsgData = std::move(m);
                }
                    break;
                case RTMP_USER_STREAM_EOF:
                case RTMP_USER_STREAM_DRY:
                case RTMP_USER_STREAM_IS_RECORDED: {
                    std::unique_ptr<StreamIDMessage> m(new StreamIDMessage(base, body.getInt()));
                    chunk->MsgData = std::move(m);
                }
                    break;
                case RTMP_USER_SET_BUFFLEN: {
                    StreamIDMessage m(base, body.getInt());
                    std::unique_ptr<SetBufferMessage> s(new SetBufferMessage(m, body.getInt()));
                    chunk->MsgData = std::move(s);
                }
                    break;
                case RTMP_USER_PING_REQUEST:

                    chunk->MsgData = std::move(
                            std::unique_ptr<PingRequestMessage>(new PingRequestMessage(base, body.getInt())));
                    break;
                case RTMP_USER_PING_RESPONSE:
                case RTMP_USER_EMPTY:
                    chunk->MsgData = std::unique_ptr<UserControlMessage>(&base);
                    break;
                default:
                    chunk->MsgData = std::unique_ptr<UserControlMessage>(&base);
                    break;
            }
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
