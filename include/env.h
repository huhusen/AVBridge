//
// Created by uma on 2023/6/5.
//

#ifndef EASYNVR_ENV_H
#define EASYNVR_ENV_H
#include <spdlog/spdlog.h>


#define PRINT_HEX(data, len) \
    do { \
        char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'}; \
        char *ascii = (char *)calloc((len) * 3 + 1, sizeof(char)); \
        int i = 0; \
        while (i < (len)) { \
            int b = ((char*)(data))[i] & 0xFF; \
            ascii[i * 2] = hex[b / 16]; \
            ascii[i * 2 + 1] = hex[b % 16]; \
            ++i; \
        } \
        std::cout << "Hex value: " << ascii << std::endl; \
        free(ascii); \
    } while (0)



#endif //EASYNVR_ENV_H
