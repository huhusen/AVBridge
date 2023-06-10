//
// Created by uma on 2023/6/7.
//

#ifndef AVBRIDGE_BIGENDIAN_H
#define AVBRIDGE_BIGENDIAN_H

#include <cstdint>
#include <vector>

class BigEndian {
public:
    static uint32_t GetIntValue(const std::vector<uint8_t> &bytes) {
        uint32_t intValue = 0;
        for (size_t i = 0; i < 4; i++) {
            intValue = (intValue << 8) | bytes[i];
        }
        return intValue;
    }

    static void GetIntValue(const std::vector<uint8_t> &b, uint32_t *num) {
        *num = 0;
        for (size_t i = 0, n = b.size(); i < n; i++) {
            *num += static_cast<uint32_t>(b[i]) << ((n - i - 1) << 3);
        }
    }
};

class LittleEndian {
public:
    static uint32_t GetLittleEndianUint32(const uint8_t *bytes) {
        uint32_t value;
        std::memcpy(&value, bytes, sizeof(uint32_t));
        return value;
    }
};

#endif //AVBRIDGE_BIGENDIAN_H
