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

    std::vector<uint8_t> Encode() override;

private:
    uint32_t m;
};

class UserControlMessage : public RtmpMessage {
public:
    explicit UserControlMessage() {}

    UserControlMessage(uint16_t eventType, const std::vector<std::uint8_t> &eventData) : EventType(eventType),
                                                                                         EventData(eventData) {}

    std::vector<uint8_t> Encode() override;

public:
    std::uint16_t EventType;
    std::vector<std::uint8_t> EventData;
};

class StreamIDMessage : public RtmpMessage {
public:
    explicit StreamIDMessage() {}

    StreamIDMessage(const UserControlMessage &userControlMessage, uint32_t streamId) : userControlMessage(
            userControlMessage), StreamID(streamId) {}

    std::vector<uint8_t> Encode() override;

public:
    UserControlMessage userControlMessage;
    std::uint32_t StreamID;
};

class SetBufferMessage : public RtmpMessage {
public:
    explicit SetBufferMessage() {}

    SetBufferMessage(const StreamIDMessage &streamIdMessage, uint32_t millisecond) : streamIdMessage(streamIdMessage),
                                                                                     Millisecond(millisecond) {}

    std::vector<uint8_t> Encode() override;

public:
    StreamIDMessage streamIdMessage;
    std::uint32_t Millisecond;
};

class PingRequestMessage : public RtmpMessage {
public:
    explicit PingRequestMessage() {}

    PingRequestMessage(const UserControlMessage &userControlMessage, uint32_t timestamp) : userControlMessage(
            userControlMessage), Timestamp(timestamp) {}

    std::vector<uint8_t> Encode() override;

public:
    UserControlMessage userControlMessage;
    std::uint32_t Timestamp;
};

class SetPeerBandwidthMessage : public RtmpMessage {
public:
    explicit SetPeerBandwidthMessage() {}


    SetPeerBandwidthMessage::SetPeerBandwidthMessage(uint32_t acknowledgementWindowsize) : AcknowledgementWindowsize(
            acknowledgementWindowsize) {}

    std::vector<uint8_t> Encode() override;

public:
    uint8_t LimitType;
    std::uint32_t AcknowledgementWindowsize;
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
            if (chunk->Body.size() < 4) {
                SPDLOG_ERROR("chunk.Body < 4");
                return;
            }
            std::unique_ptr<SetPeerBandwidthMessage> m(new SetPeerBandwidthMessage(body.getInt()));
            if (body.getSize() > 0) {
                m->LimitType = body.getBuffer()[0];
            }
            chunk->MsgData = std::move(m);
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
