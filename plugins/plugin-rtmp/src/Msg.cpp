//
// Created by uma on 2023/6/12.
//

#include "Msg.h"
#include "core/ByteBuffer.h"

std::vector<uint8_t> Uint32Message::Encode() {
    std::vector<uint8_t> b(4);
    uint32_t value = static_cast<uint32_t>(m);
    b[0] = static_cast<uint8_t>((value >> 24) & 0xFF);
    b[1] = static_cast<uint8_t>((value >> 16) & 0xFF);
    b[2] = static_cast<uint8_t>((value >> 8) & 0xFF);
    b[3] = static_cast<uint8_t>(value & 0xFF);
    return b;
}

std::vector<uint8_t> UserControlMessage::Encode() {
    std::vector<unsigned char> b;
    for (size_t i = 0, n = 2; i < n; i++) {
        b[i] = static_cast<unsigned char>(EventType >> ((n - i - 1) << 3));
    }
    return b;
}

std::vector<uint8_t> StreamIDMessage::Encode() {
    ByteBuffer bf(6);
    bf.putShort(userControlMessage.EventType);
    bf.putInt(StreamID);
    userControlMessage.EventData = bf.getRange(2, bf.getPosition());
    return bf.getBuffer();
}

std::vector<uint8_t> SetBufferMessage::Encode() {
    ByteBuffer bf(10);
    bf.putShort(streamIdMessage.userControlMessage.EventType);
    bf.putInt(streamIdMessage.StreamID);
    bf.putInt(Millisecond);
    streamIdMessage.userControlMessage.EventData = bf.getRange(2, bf.getPosition());
    return bf.getBuffer();
}

std::vector<uint8_t> PingRequestMessage::Encode() {
    ByteBuffer bf(6);
    bf.putShort(userControlMessage.EventType);
    bf.putInt(Timestamp);
    userControlMessage.EventData = bf.getRange(2, bf.getPosition());
    return bf.getBuffer();
}

std::vector<uint8_t> SetPeerBandwidthMessage::Encode() {
    ByteBuffer bf(5);
    bf.putInt(AcknowledgementWindowsize);
    bf.put(LimitType);
    return bf.getBuffer();
}

