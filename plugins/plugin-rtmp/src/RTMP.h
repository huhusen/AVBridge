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
    RTMP_MAX_CHUNK_SIZE = 65536

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
