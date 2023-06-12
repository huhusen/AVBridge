//
// Created by uma on 2023/6/10.
//

#include "RTMPClient.h"
#include "Msg.h"

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
    ChunkHeader h;
    if (rtmpHeader.count(ChunkStreamID) == 0) {
        ChunkHeader chunkHeader;
        ChunkBasicHeader chunkBasicHeader;
        chunkBasicHeader.chunkStreamID = ChunkStreamID;
        chunkBasicHeader.chunkType = ChunkType;
        chunkHeader.basicHeader = chunkBasicHeader;
        this->rtmpHeader[ChunkStreamID] = chunkHeader;
        h = chunkHeader;
    } else {
        h = rtmpHeader[ChunkStreamID];
    }
    bool ok = incompleteRtmpBody.count(ChunkStreamID) != 0;
    ByteBuffer currentBody = incompleteRtmpBody[ChunkStreamID];
    if (ChunkType != 3 && ok && currentBody.getSize() > 0) {
        SPDLOG_ERROR("incompleteRtmpBody error");
        return;
    }
    this->readChunkType(h, ChunkType);

    uint32_t msgLen = h.messageHeader.messageLength;
    if (!ok) {

        currentBody.resize(msgLen);
        incompleteRtmpBody[ChunkStreamID] = currentBody;
    }
    int needRead = this->readChunkSize;

    int unRead = msgLen - currentBody.getPosition();
    if (unRead < needRead) {
        needRead = unRead;
    }
    std::vector<uint8_t> _currentBody(needRead);
    this->inMsg->getBytes(_currentBody.data(), _currentBody.size());
    currentBody.putBytes(_currentBody.data(), _currentBody.size());
    this->readSeqNum += currentBody.getSize();
    if (currentBody.getPosition() == msgLen) {
        Chunk msg;
        msg.Header = h;
        currentBody.getBytes(msg.Body.data(), msgLen);
        GetRtmpMessage(&msg);
    }


}

void RTMPClient::readChunkType(ChunkHeader &h, uint8_t chunkType) {
    std::vector<uint8_t> b(3);
    if (chunkType == 0) {
        this->inMsg->getBytes(b.data(), b.size());
        this->readSeqNum += 3;
        BigEndian::GetIntValue(b, &h.messageHeader.timestamp);
        this->inMsg->getBytes(b.data(), b.size());
        this->readSeqNum += 3;
        BigEndian::GetIntValue(b, &h.messageHeader.messageLength);
        uint8_t v = this->inMsg->get();
        this->readSeqNum++;
        h.messageHeader.messageTypeID = v;
        b.clear();
        b.resize(4);
        this->inMsg->getBytes(b.data(), b.size());
        this->readSeqNum += 4;
        h.messageHeader.messageStreamID = LittleEndian::GetLittleEndianUint32(b.data());
        if (h.messageHeader.timestamp == 0xffffff) {
            this->inMsg->getBytes(b.data(), 4);
            this->readSeqNum += 4;
            BigEndian::GetIntValue(b, &h.extendTimestamp);
        }
        this->tmpBuf->putBytes(b.data(), 4, 0);

    } else if (chunkType == 1) {
        SPDLOG_ERROR("");
    } else if (chunkType == 2) {
        // 执行 chunkType 为 2 的操作
    } else if (chunkType == 3) {
        // 执行 chunkType 为 3 的操作
    } else {
        // 默认情况下的操作
    }

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