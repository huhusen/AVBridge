//
// Created by uma on 2023/6/10.
//

#include "RTMPClient.h"

void RTMPClient::Execute(hv::Buffer *buf) {
    this->inMsg->init(buf->data(), buf->size());

    if (this->lastStatus == New) {
        const RTMPException &exception = this->handshake.execute(*inMsg, *outMsg);
        if (exception.errorMessage == "") {
            this->channel->write(outMsg->getBuffer().data(), outMsg->getSize());
            this->lastStatus = C0C1;
            SPDLOG_INFO("COC1 Done.");
        } else {
            SPDLOG_ERROR(exception.errorMessage);
        }
        return;
    }
    if (this->lastStatus == C0C1 && buf->size() == 1536) {
        this->lastStatus = C2;
        SPDLOG_INFO("C2 Done.");
        return;
    }
    if (this->lastStatus == C2) {
        HandleMsg();
    }


}

void RTMPClient::HandleMsg() {
    if (this->readSeqNum >= this->bandwidth) {
        this->totalRead += this->readSeqNum;
        this->readSeqNum = 0;
        this->SendMsg();
    }
    this->readChunk();
}

void RTMPClient::readChunk() {
    uint8_t head = this->inMsg->get();
    this->readSeqNum++;
    uint32_t ChunkStreamID = head & 0x3f;
    uint8_t ChunkType = head >> 6;
    ChunkStreamID = this->readChunkStreamID(ChunkStreamID);
    ChunkHeader &h = rtmpHeader[ChunkStreamID];
    if (rtmpHeader.count(ChunkStreamID) == 0) {
        ChunkHeader chunkHeader;
        ChunkBasicHeader chunkBasicHeader;
        chunkBasicHeader.chunkStreamID = ChunkStreamID;
        chunkBasicHeader.chunkType = ChunkType;
        chunkHeader.basicHeader = chunkBasicHeader;
        this->rtmpHeader[ChunkStreamID] = chunkHeader;
        h = chunkHeader;
    }
    ByteBuffer currentBody = incompleteRtmpBody[ChunkStreamID];
    if (ChunkType != 3 && currentBody.getSize() > 0) {
        SPDLOG_ERROR("incompleteRtmpBody error");
        return;
    }


    uint32_t msgLen = h.messageHeader.messageLength;

    SPDLOG_INFO("");

}

uint32_t RTMPClient::readChunkStreamID(uint32_t csid) {
    uint32_t chunkStreamID = csid;

    switch (csid) {
        case 0: {
            uint8_t u8 = this->inMsg->get();
            this->readSeqNum++;
            chunkStreamID = 64 + static_cast<uint32_t>(u8);
            break;
        }
        case 1: {
            uint8_t u16_0 = this->inMsg->get();
            uint8_t u16_1 = this->inMsg->get();
            this->readSeqNum += 2;
            chunkStreamID = 64 + static_cast<uint32_t>(u16_0) + (static_cast<uint32_t>(u16_1) << 8);
            break;
        }
    }
    return chunkStreamID;
}

void RTMPClient::SendMsg() {

}