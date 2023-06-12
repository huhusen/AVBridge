//
// Created by uma on 2023/6/10.
//

#ifndef AVBRIDGE_RTMP_H
#define AVBRIDGE_RTMP_H


enum {
    RTMP_HANDSHAKE_VERSION = 0x03,
    C1S1_SIZE = 1536,
    C1S1_DIGEST_OFFSET_MAX = 764 - 32 - 4,
    C1S1_TIME_SIZE = 4,
    C1S1_VERSION_SIZE = 4,
    C1S1_DIGEST_OFFSET_SIZE = 4,
    C1S1_KEY_OFFSET_MAX = 764 - 128 - 4,
    C1S1_DIGEST_SIZE = 764,
    C1S1_KEY_SIZE = 764,
    C1S1_DIGEST_DATA_SIZE = 32,
    C1S1_KEY_DATA_SIZE = 128,
    RTMP_MAX_CHUNK_SIZE = 65536,
    RTMP_DEFAULT_CHUNK_SIZE = 128,

    // Chunk
    RTMP_MSG_CHUNK_SIZE = 1,
    RTMP_MSG_ABORT = 2,

    // RTMP
    RTMP_MSG_ACK = 3,
    RTMP_MSG_USER_CONTROL = 4,
    RTMP_MSG_ACK_SIZE = 5,
    RTMP_MSG_BANDWIDTH = 6,
    RTMP_MSG_EDGE = 7,
    RTMP_MSG_AUDIO = 8,
    RTMP_MSG_VIDEO = 9,
    RTMP_MSG_AMF3_METADATA = 15,
    RTMP_MSG_AMF3_SHARED = 16,
    RTMP_MSG_AMF3_COMMAND = 17,

    RTMP_MSG_AMF0_METADATA = 18,
    RTMP_MSG_AMF0_SHARED = 19,
    RTMP_MSG_AMF0_COMMAND = 20,

    RTMP_MSG_AGGREGATE = 22,

    RTMP_MAX_CHUNK_HEADER = 18,

    // User Control Event
    RTMP_USER_STREAM_BEGIN = 0,
    RTMP_USER_STREAM_EOF = 1,
    RTMP_USER_STREAM_DRY = 2,
    RTMP_USER_SET_BUFFLEN = 3,
    RTMP_USER_STREAM_IS_RECORDED = 4,
    RTMP_USER_PING_REQUEST = 6,
    RTMP_USER_PING_RESPONSE = 7,
    RTMP_USER_EMPTY = 31,

    // StreamID == (ChannelID-4)/5+1
    // ChannelID == Chunk Stream ID
    // StreamID == Message Stream ID
    // Chunk Stream ID == 0, �ڶ���byte + 64
    // Chunk Stream ID == 1, (������byte) * 256 + �ڶ���byte + 64
    // Chunk Stream ID == 2.
    // 2 < Chunk Stream ID < 64(2��6�η�)
    RTMP_CSID_CONTROL = 0x02,
    RTMP_CSID_COMMAND = 0x03,
    RTMP_CSID_AUDIO = 0x06,
    RTMP_CSID_DATA = 0x05,
    RTMP_CSID_VIDEO = 0x05
};
struct ChunkBasicHeader {
    uint32_t chunkStreamID;
    uint8_t chunkType;
};

struct ChunkMessageHeader {
    uint32_t timestamp;
    uint32_t messageLength;
    uint8_t messageTypeID;
    uint32_t messageStreamID;
};
struct ChunkHeader {
    ChunkBasicHeader basicHeader;
    ChunkMessageHeader messageHeader;
    uint32_t extendTimestamp;
};




#endif //AVBRIDGE_RTMP_H
