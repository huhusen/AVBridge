//
// Created by uma on 2023/6/8.
//


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
RTMPException Handshake::execute(void *buf, size_t len) {

    uint8_t *uint8Ptr = static_cast<uint8_t *>(buf);
    std::vector<uint8_t> bytes(uint8Ptr, uint8Ptr + len);
    if (bytes[0] != RTMP_HANDSHAKE_VERSION) {
        return RTMPException("C0 Error");
    }
    SLICE_START(C1, bytes, 1);
    if (C1.size() != C1S1_SIZE) {
        return RTMPException("C1 Error");
    }
    SLICE_RANGE(tsbs, C1.Vector(), 4, 8);
    uint32_t ts = BigEndian::GetIntValue(tsbs.Vector());
    if (ts == 0) {
        return this->simpleHandShake.execute(C1.Vector());
    }
    return this->complexHandShake.execute(C1.Vector());
}


RTMPException SimpleHandShake::execute(std::vector<uint8_t> C1) {
    return nullptr;
}


RTMPException ComplexHandShake::execute(std::vector<uint8_t> C1) {
    const ClientSchemeInfo &clientSchemeInfo = validateClient(C1);
    if (clientSchemeInfo.error != "") {
        return RTMPException(clientSchemeInfo.error.data());
    }
    return RTMPException("");
}


ComplexHandShake::ClientSchemeInfo ComplexHandShake::validateClient(std::vector<uint8_t> C1) {
    ClientSchemeInfo clientSchemeInfo = clientScheme(C1, 1);
    if (clientSchemeInfo.ok) {
        return clientSchemeInfo;
    }
    clientSchemeInfo = clientScheme(C1, 0);
    if (clientSchemeInfo.ok) {
        return clientSchemeInfo;
    }
    clientSchemeInfo.error = "Client scheme error";
    return clientSchemeInfo;
}

ComplexHandShake::ClientSchemeInfo ComplexHandShake::clientScheme(std::vector<uint8_t> C1, int scheme) {
    int digest_offset = -1;
    int key_offset = -1;
    if (scheme == 0) {
        digest_offset = scheme0_Digest_Offset(C1);
        key_offset = scheme0_Key_Offset(C1);
    } else if (scheme == 1) {
        digest_offset = scheme1_Digest_Offset(C1);
        key_offset = scheme1_Key_Offset(C1);
    }
    SLICE_RANGE(c1_Part1, C1, 0, digest_offset);
    SLICE_START(c1_Part2, C1, digest_offset + C1S1_DIGEST_DATA_SIZE);
    SLICE_RANGE(digest, C1, digest_offset, digest_offset + C1S1_DIGEST_DATA_SIZE);
    std::vector<uint8_t> c1_Part1_Part2;
    const std::vector<uint8_t> &tempc1_Part1 = c1_Part1.Vector();
    const std::vector<uint8_t> &tempc1_Part2 = c1_Part2.Vector();
    c1_Part1_Part2.insert(c1_Part1_Part2.end(), tempc1_Part1.begin(), tempc1_Part1.end());
    c1_Part1_Part2.insert(c1_Part1_Part2.end(), tempc1_Part2.begin(), tempc1_Part2.end());

    SLICE_RANGE(key, FP_KEY, 0, 30);
    std::vector<uint8_t> tmp_Hash = Crypto::HmacSha256::Calculate(c1_Part1_Part2, key.Vector());

    bool ok = false;
    if (std::memcmp(digest.Vector().data(), tmp_Hash.data(), digest.size()) == 0) {
        ok = true;
    }
    SLICE_RANGE(challenge, C1, key_offset, key_offset + C1S1_KEY_DATA_SIZE);
    return {scheme, challenge.Vector(), digest.Vector(), ok};

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