//
// Created by uma on 2023/6/8.
//


#include <random>
#include "HandShake.h"
#include "core/Crypto.h"

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
    C1S1_KEY_DATA_SIZE = 128

};
const std::vector<uint8_t> FMS_KEY = {
        0x47, 0x65, 0x6e, 0x75, 0x69, 0x6e, 0x65, 0x20,
        0x41, 0x64, 0x6f, 0x62, 0x65, 0x20, 0x46, 0x6c,
        0x61, 0x73, 0x68, 0x20, 0x4d, 0x65, 0x64, 0x69,
        0x61, 0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72,
        0x20, 0x30, 0x30, 0x31, // Genuine Adobe Flash Media Server 001
        0xf0, 0xee, 0xc2, 0x4a, 0x80, 0x68, 0xbe, 0xe8,
        0x2e, 0x00, 0xd0, 0xd1, 0x02, 0x9e, 0x7e, 0x57,
        0x6e, 0xec, 0x5d, 0x2d, 0x29, 0x80, 0x6f, 0xab,
        0x93, 0xb8, 0xe6, 0x36, 0xcf, 0xeb, 0x31, 0xae
}; // 68

const std::vector<uint8_t> FP_KEY = {
        0x47, 0x65, 0x6E, 0x75, 0x69, 0x6E, 0x65, 0x20,
        0x41, 0x64, 0x6F, 0x62, 0x65, 0x20, 0x46, 0x6C,
        0x61, 0x73, 0x68, 0x20, 0x50, 0x6C, 0x61, 0x79,
        0x65, 0x72, 0x20, 0x30, 0x30, 0x31, /* Genuine Adobe Flash Player 001 */
        0xF0, 0xEE, 0xC2, 0x4A, 0x80, 0x68, 0xBE, 0xE8,
        0x2E, 0x00, 0xD0, 0xD1, 0x02, 0x9E, 0x7E, 0x57,
        0x6E, 0xEC, 0x5D, 0x2D, 0x29, 0x80, 0x6F, 0xAB,
        0x93, 0xB8, 0xE6, 0x36, 0xCF, 0xEB, 0x31, 0xAE
}; // 62
RTMPException Handshake::execute(const hv::SocketChannelPtr &channel, hv::Buffer *buf) {

    ByteBuffer byteBuffer(buf->data(), buf->size());
    if (byteBuffer.get() != RTMP_HANDSHAKE_VERSION) {
        return RTMPException("C0 Error");
    }
    ByteBuffer C1Buffer(byteBuffer.getRange(1, byteBuffer.getSize()));
    if (C1Buffer.getSize() != C1S1_SIZE) {
        return RTMPException("C1 Error");
    }


    auto ts = C1Buffer.getInt(4);;
    if (ts == 0) {
        return this->simpleHandShake.execute(C1Buffer, channel);
    }
    return this->complexHandShake.execute(C1Buffer, channel);
}


RTMPException SimpleHandShake::execute(ByteBuffer C1, const hv::SocketChannelPtr &channel) {
    return nullptr;
}


RTMPException ComplexHandShake::execute(ByteBuffer C1, const hv::SocketChannelPtr &channel) {
    const ClientSchemeInfo &clientSchemeInfo = validateClient(C1);
    if (!clientSchemeInfo.ok) {
        return RTMPException("Client scheme error");
    }

    ByteBuffer S1(create_S1());

    int S1_Digest_Offset = scheme_Digest_Offset(S1.getBuffer(), clientSchemeInfo.scheme);
    const std::vector<uint8_t> &S1_Part1 = S1.getRange(0, S1_Digest_Offset);
    const std::vector<uint8_t> &S1_Part2 = S1.getRange(S1_Digest_Offset + C1S1_DIGEST_DATA_SIZE, S1.getSize());
    ByteBuffer S1_Part1_Part2(S1_Part1.size() + S1_Part2.size());
    S1_Part1_Part2.putBytes(S1_Part1.data(), S1_Part1.size());
    S1_Part1_Part2.putBytes(S1_Part2.data(), S1_Part2.size());
    SLICE_RANGE(key, FP_KEY, 0, 36);
    std::vector<uint8_t> tmp_Hash = Crypto::HmacSha256::Calculate(S1_Part1_Part2.getBuffer(), key.Vector());
    S1.putBytes(tmp_Hash.data(), tmp_Hash.size(), S1_Digest_Offset);

    ByteBuffer S2_Random(create_S2());

    SLICE_RANGE(key2, FMS_KEY, 0, 68);

    tmp_Hash = Crypto::HmacSha256::Calculate(clientSchemeInfo.digest, key2.Vector());


    const std::vector<uint8_t> &S2_Digest = Crypto::HmacSha256::Calculate(S2_Random.getBuffer(), tmp_Hash);
    ByteBuffer receive((1 + S1.getSize() + S2_Random.getSize() + S2_Digest.size()),ByteBuffer::Endian::Big);
    receive.put(RTMP_HANDSHAKE_VERSION);
    receive.putBytes(S1.getBuffer().data(), S1.getSize());
    receive.putBytes(S2_Random.getBuffer().data(), S2_Random.getSize());
    receive.putBytes(S2_Digest.data(), S2_Digest.size());
    channel->write(receive.getBuffer().data(), receive.getSize());

    return RTMPException("");
}

int ComplexHandShake::scheme_Digest_Offset(const std::vector<uint8_t> &C1S1, int scheme) {
    if (scheme == 0) {
        return scheme0_Digest_Offset(C1S1);
    } else if (scheme == 1) {
        return scheme1_Digest_Offset(C1S1);
    }
    return -1;
}

std::vector<uint8_t> ComplexHandShake::create_S1() {
    std::vector<uint8_t> s1_Time(4, 0);
    std::vector<uint8_t> s1_Version(4, 1);
    std::vector<uint8_t> s1_key_Digest(1536 - 8);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    for (auto &byte: s1_key_Digest) {
        byte = static_cast<uint8_t>(dis(gen));
    }
    std::vector<uint8_t> result;
    result.reserve(s1_Time.size() + s1_Version.size() + s1_key_Digest.size());
    result.insert(result.end(), s1_Time.begin(), s1_Time.end());
    result.insert(result.end(), s1_Version.begin(), s1_Version.end());
    result.insert(result.end(), s1_key_Digest.begin(), s1_key_Digest.end());
    return result;
}

std::vector<uint8_t> ComplexHandShake::create_S2() {
    std::vector<uint8_t> s2_Random(1536 - 32);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    for (auto &byte: s2_Random) {
        byte = static_cast<uint8_t>(dis(gen));
    }
    return s2_Random;
}


ComplexHandShake::ClientSchemeInfo ComplexHandShake::validateClient(ByteBuffer C1) {
    ClientSchemeInfo clientSchemeInfo = clientScheme(C1, 1);
    if (clientSchemeInfo.ok) {
        return clientSchemeInfo;
    }
    clientSchemeInfo = clientScheme(C1, 0);
    if (clientSchemeInfo.ok) {
        return clientSchemeInfo;
    }
    return clientSchemeInfo;
}

ComplexHandShake::ClientSchemeInfo ComplexHandShake::clientScheme(ByteBuffer C1, int scheme) {
    int digest_offset = -1;
    int key_offset = -1;
    if (scheme == 0) {
        digest_offset = scheme0_Digest_Offset(C1.getBuffer());
        key_offset = scheme0_Key_Offset(C1.getBuffer());
    } else if (scheme == 1) {
        digest_offset = scheme1_Digest_Offset(C1.getBuffer());
        key_offset = scheme1_Key_Offset(C1.getBuffer());
    }
    const std::vector<uint8_t> &c1_Part1 = C1.getRange(0, digest_offset);
    const std::vector<uint8_t> &c1_Part2 = C1.getRange(digest_offset + C1S1_DIGEST_DATA_SIZE, C1.getSize());
    const std::vector<uint8_t> &digest = C1.getRange(digest_offset, digest_offset + C1S1_DIGEST_DATA_SIZE);

    std::vector<uint8_t> c1_Part1_Part2;

    c1_Part1_Part2.insert(c1_Part1_Part2.end(), c1_Part1.begin(), c1_Part1.end());
    c1_Part1_Part2.insert(c1_Part1_Part2.end(), c1_Part2.begin(), c1_Part2.end());

    SLICE_RANGE(key, FP_KEY, 0, 30);
    std::vector<uint8_t> tmp_Hash = Crypto::HmacSha256::Calculate(c1_Part1_Part2, key.Vector());

    bool ok = false;
    if (std::memcmp(digest.data(), tmp_Hash.data(), digest.size()) == 0) {
        ok = true;
    }
    const std::vector<uint8_t> &challenge = C1.getRange(key_offset, key_offset + C1S1_KEY_DATA_SIZE);

    return {scheme, challenge, digest, ok};

}

// scheme0:
// time + version + digest 										  + key
// time + version + [offset + random + digest-data + random-data] + key
// 4	+ 4 	  + [4		+ offset + 32		   + 728-offset ] + 764
// 4 	+ 4		  + 764											  + 764
// 0 <= scheme0_digest_offset <= 728 == 764 - 32 - 4
// 如果digest.offset == 3,那么digest[7~38]为digest.digest-data,如果offset == 728, 那么digest[732~763]为digest-data)
int ComplexHandShake::scheme0_Digest_Offset(std::vector<uint8_t> C1S1) {
    int scheme0_digest_offset = static_cast<int>(C1S1[8] & 0xff) +
                                static_cast<int>(C1S1[9] & 0xff) +
                                static_cast<int>(C1S1[10] & 0xff) +
                                static_cast<int>(C1S1[11] & 0xff);

    scheme0_digest_offset = (scheme0_digest_offset % C1S1_DIGEST_OFFSET_MAX) + C1S1_TIME_SIZE + C1S1_VERSION_SIZE +
                            C1S1_DIGEST_OFFSET_SIZE;
    if (scheme0_digest_offset + 32 >= C1S1_SIZE) {
        // digest error
        // digest 数据超出1536.
    }

    return scheme0_digest_offset;
}

// scheme1:
// time + version + key + digest
// 0 <= scheme1_digest_offset <= 728 == 764 - 32 - 4
int ComplexHandShake::scheme1_Digest_Offset(std::vector<uint8_t> C1S1) {
    int scheme1_digest_offset = static_cast<int>(C1S1[772] & 0xff) +
                                static_cast<int>(C1S1[773] & 0xff) +
                                static_cast<int>(C1S1[774] & 0xff) +
                                static_cast<int>(C1S1[775] & 0xff);

    scheme1_digest_offset =
            (scheme1_digest_offset % C1S1_DIGEST_OFFSET_MAX) + C1S1_TIME_SIZE + C1S1_VERSION_SIZE + C1S1_KEY_SIZE +
            C1S1_DIGEST_OFFSET_SIZE;
    if (scheme1_digest_offset + 32 >= C1S1_SIZE) {
        // digest error
    }

    return scheme1_digest_offset;
}

// key:
// random-data + key-data + random-data 		+ offset
// offset	   + 128	  +	764-offset-128-4	+ 4
// 0 <= scheme0_key_offset <= 632 == 764 - 128 - 4
// 如果key.offset == 3, 那么key[3~130]为key-data,这个位置是相对于key结构的第0个字节开始
int ComplexHandShake::scheme0_Key_Offset(std::vector<uint8_t> C1S1) {
    int scheme0_key_offset = static_cast<int>(C1S1[1532]) +
                             static_cast<int>(C1S1[1533]) +
                             static_cast<int>(C1S1[1534]) +
                             static_cast<int>(C1S1[1535]);

    scheme0_key_offset =
            (scheme0_key_offset % C1S1_KEY_OFFSET_MAX) + C1S1_TIME_SIZE + C1S1_VERSION_SIZE + C1S1_DIGEST_SIZE;
    if (scheme0_key_offset + 128 >= C1S1_SIZE) {
        // key error
    }

    return scheme0_key_offset;
}

// time + version + key + digest
// 0 <= scheme1_key_offset <= 632 == 764 - 128 - 4
int ComplexHandShake::scheme1_Key_Offset(std::vector<uint8_t> C1S1) {
    int scheme1_key_offset = static_cast<int>(C1S1[768]) +
                             static_cast<int>(C1S1[769]) +
                             static_cast<int>(C1S1[770]) +
                             static_cast<int>(C1S1[771]);

    scheme1_key_offset =
            (scheme1_key_offset % C1S1_KEY_OFFSET_MAX) + C1S1_TIME_SIZE + C1S1_VERSION_SIZE + C1S1_DIGEST_SIZE;
    if (scheme1_key_offset + 128 >= C1S1_SIZE) {
        // key error
    }

    return scheme1_key_offset;
}