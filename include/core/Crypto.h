//
// Created by uma on 2023/6/8.
//

#ifndef AVBRIDGE_CRYPTO_H
#define AVBRIDGE_CRYPTO_H

#include <iostream>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <vector>
#include <cstdint>
#include <cstring>

namespace Crypto {
    class HmacSha256 {
    public:
        static std::vector<uint8_t> Calculate(const std::vector<uint8_t> &message,const std::vector<uint8_t> &key) {
            unsigned char digest[EVP_MAX_MD_SIZE];
            unsigned int len;

            HMAC_CTX *ctx = HMAC_CTX_new();
            HMAC_Init_ex(ctx, key.data(), key.size(), EVP_sha256(), nullptr);
            HMAC_Update(ctx, message.data(), message.size());
            HMAC_Final(ctx, digest, &len);
            HMAC_CTX_free(ctx);

            return std::vector<uint8_t>(digest, digest + len);
        }
    };
}


#endif //AVBRIDGE_CRYPTO_H
