//
// Created by uma on 2023/6/7.
//

#ifndef AVBRIDGE_SLICE_H
#define AVBRIDGE_SLICE_H

#include <iostream>
#include <vector>
#include <iostream>
#include <vector>

template<typename T>
class Slice {
private:
    std::vector<T> data;
    size_t start;
    size_t end;

public:
    Slice(const std::vector<T> &arr, size_t start, size_t end)
            : data(arr), start(start), end(end) {}

    Slice(const std::vector<T> &arr, size_t start)
            : data(arr), start(start), end(arr.size()) {}

    Slice(const std::vector<T> &arr)
            : data(arr), start(0), end(arr.size()) {}

    T &operator[](size_t index) {
        if (index >= start && index < end) {
            return data[index];
        } else {
            throw std::out_of_range("Index out of range");
        }
    }

    size_t size() const {
        return end - start;
    }

    std::vector<T> Vector() const {
        return std::vector<T>(data.begin() + start, data.begin() + end);
    }
};

#define SLICE_START(name, arr, start) \
    Slice<uint8_t> name(arr, start)

#define SLICE_RANGE(name, arr, start, end) \
    Slice<uint8_t> name(arr, start, end)

#endif //AVBRIDGE_SLICE_H
