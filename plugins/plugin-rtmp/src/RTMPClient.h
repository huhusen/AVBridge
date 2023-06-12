//
// Created by uma on 2023/6/10.
//

#ifndef AVBRIDGE_RTMPCLIENT_H
#define AVBRIDGE_RTMPCLIENT_H

#include "HandShake.h"
#include "RTMP.h"


class RTMPClient {
    enum STATUS {
        New, C0C1, C2
    };
public:
    RTMPClient(const std::shared_ptr<hv::SocketChannel> &channel) : channel(channel) {
        inMsg = new ByteBuffer();
        outMsg = new ByteBuffer();
        tmpBuf = new ByteBuffer(4);
    };

    ~RTMPClient() {
        SPDLOG_INFO("~RtmpClient");
        delete inMsg;
        delete outMsg;
    }

    void Execute(hv::Buffer *buf);


    const hv::SocketChannelPtr &channel;
private:
    void HandleMsg();

    void SendMsg();

    //
    void readChunk();

    void readChunkType(ChunkHeader &h, uint8_t chunkType);

    uint32_t readChunkStreamID(uint32_t csid);

private:
    Handshake handshake;
    STATUS lastStatus = New;
    ByteBuffer *inMsg;
    ByteBuffer *outMsg;
    ByteBuffer *tmpBuf;

    uint32_t bandwidth = RTMP_MAX_CHUNK_SIZE << 3;
    uint32_t readSeqNum = 0;
    uint32_t writeSeqNum;
    uint32_t totalWrite;
    uint32_t totalRead = 0;
    int writeChunkSize = 4096; // 写入块大小
    int readChunkSize = RTMP_DEFAULT_CHUNK_SIZE; // 读取块大小


    std::map<uint32_t, ChunkHeader> rtmpHeader;
    std::map<uint32_t, ByteBuffer> incompleteRtmpBody;


};


#endif //AVBRIDGE_RTMPCLIENT_H
