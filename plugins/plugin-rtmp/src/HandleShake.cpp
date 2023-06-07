//
// Created by uma on 2023/6/7.
//
#include "PluginRTMP.h"


enum {
    RTMP_HANDSHAKE_VERSION = 0x03,
    C1S1_SIZE = 1536,
};

RTMPException *PluginRTMP::Handshake(void *buf, size_t len) {

    uint8_t *uint8Ptr = static_cast<uint8_t *>(buf);
    std::vector <uint8_t> bytes(uint8Ptr, uint8Ptr + len);
    if (bytes[0] != RTMP_HANDSHAKE_VERSION) {
        return new RTMPException("C0 Error");
    }
    SLICE_START(C1, bytes, 1);
    if (C1.size() != C1S1_SIZE) {
        return new RTMPException("C1 Error");
    }
    SLICE_RANGE(tsbs, C1.Vector(), 4, 8);
    uint32_t ts = BigEndian::GetIntValue(tsbs.Vector());
    if (ts == 0) {
        SPDLOG_INFO("123");
    } else {
        SPDLOG_INFO("456");
    }
    return nullptr;
}