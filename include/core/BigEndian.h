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
};

#endif //AVBRIDGE_BIGENDIAN_H
