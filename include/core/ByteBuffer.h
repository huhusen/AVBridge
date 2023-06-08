//
// Created by uma on 2023/6/8.
//

#ifndef AVBRIDGE_BYTEBUFFER_H
#define AVBRIDGE_BYTEBUFFER_H

#include <vector>
#include <cstring>
#include <stdexcept>

#ifdef _WIN32

#include <winsock2.h>

#else
#include <arpa/inet.h>
#endif


class ByteBuffer {
public:
    enum class Endian {
        Big,
        Little
    };

    ByteBuffer(Endian endian = Endian::Big) : endian_(endian), position_(0) {}

    ByteBuffer(const std::vector<uint8_t> &data, Endian endian = Endian::Big)
            : endian_(endian), buffer_(data), position_(0) {}


    ByteBuffer(uint32_t size,Endian endian = Endian::Big) : endian_(endian), position_(0) {
        buffer_.resize(size);
    }

    ByteBuffer(void *buf, uint32_t size, Endian endian = Endian::Big)
            : endian_(endian), position_(0) {
        buffer_.resize(size);
        std::memcpy(buffer_.data(), buf, size);
    }


    ~ByteBuffer() {
        clear();
    }

    void reset() {
        clear();
    }
    void setEndian(Endian endian ){
        this->endian_=endian;
    }

    std::vector<uint8_t> getBuffer() const {
        return buffer_;
    }

    ByteBuffer &put(const ByteBuffer &other) {
        return putBytes(other.buffer_.data(), other.buffer_.size());
    }

    ByteBuffer &put(uint8_t value) {
        return putBytes(&value, sizeof(value));
    }

    ByteBuffer &put(uint8_t value, uint32_t index) {
        checkIndex(index);
        buffer_[index] = value;
        return *this;
    }

    ByteBuffer &putBytes(const uint8_t *buf, uint32_t len) {
        checkRemaining(len);
        std::copy(buf, buf + len, buffer_.begin() + position_);
        position_ += len;
        return *this;
    }

    ByteBuffer &putBytes(const uint8_t *buf, uint32_t len, uint32_t index) {
        checkIndex(index);
        checkRemaining(len);
        std::copy(buf, buf + len, buffer_.begin() + index);
        return *this;
    }

    ByteBuffer &putChar(char value) {
        return put(static_cast<uint8_t>(value));
    }

    ByteBuffer &putChar(char value, uint32_t index) {
        return put(static_cast<uint8_t>(value), index);
    }

    ByteBuffer &putShort(uint16_t value) {
        value = convertEndian(value);
        return putBytes(reinterpret_cast<const uint8_t *>(&value), sizeof(value));
    }

    ByteBuffer &putShort(uint16_t value, uint32_t index) {
        value = convertEndian(value);
        return putBytes(reinterpret_cast<const uint8_t *>(&value), sizeof(value), index);
    }

    ByteBuffer &putInt(uint32_t value) {
        value = convertEndian(value);
        return putBytes(reinterpret_cast<const uint8_t *>(&value), sizeof(value));
    }

    ByteBuffer &putInt(uint32_t value, uint32_t index) {
        value = convertEndian(value);
        return putBytes(reinterpret_cast<const uint8_t *>(&value), sizeof(value), index);
    }

    ByteBuffer &putLong(uint64_t value) {
        value = convertEndian(value);
        return putBytes(reinterpret_cast<const uint8_t *>(&value), sizeof(value));
    }

    ByteBuffer &putLong(uint64_t value, uint32_t index) {
        value = convertEndian(value);
        return putBytes(reinterpret_cast<const uint8_t *>(&value), sizeof(value), index);
    }

    ByteBuffer &putFloat(float value) {
        uint32_t intValue = *reinterpret_cast<uint32_t *>(&value);
        intValue = convertEndian(intValue);
        return putBytes(reinterpret_cast<const uint8_t *>(&intValue), sizeof(intValue));
    }

    ByteBuffer &putFloat(float value, uint32_t index) {
        uint32_t intValue = *reinterpret_cast<uint32_t *>(&value);
        intValue = convertEndian(intValue);
        return putBytes(reinterpret_cast<const uint8_t *>(&intValue), sizeof(intValue), index);
    }

    ByteBuffer &putDouble(double value) {
        uint64_t longValue = *reinterpret_cast<uint64_t *>(&value);
        longValue = convertEndian(longValue);
        return putBytes(reinterpret_cast<const uint8_t *>(&longValue), sizeof(longValue));
    }

    ByteBuffer &putDouble(double value, uint32_t index) {
        uint64_t longValue = *reinterpret_cast<uint64_t *>(&value);
        longValue = convertEndian(longValue);
        return putBytes(reinterpret_cast<const uint8_t *>(&longValue), sizeof(longValue), index);
    }

    uint8_t get() {
        return getByte(position_++);
    }

    uint8_t get(uint32_t index) const {
        checkIndex(index);
        return getByte(index);
    }

    void getBytes(uint8_t *buf, uint32_t len) {
        getBytes(position_, buf, len);
        position_ += len;
    }

    void getBytes(uint32_t index, uint8_t *buf, uint32_t len) const {
        checkIndex(index);
        checkIndex(index + len - 1);
        std::copy(buffer_.begin() + index, buffer_.begin() + index + len, buf);
    }

    char getChar() {
        return static_cast<char>(get());
    }

    char getChar(uint32_t index) const {
        return static_cast<char>(get(index));
    }

    uint16_t getShort() {
        uint16_t value;
        getBytes(reinterpret_cast<uint8_t *>(&value), sizeof(value));
        return convertEndian(value);
    }

    uint16_t getShort(uint32_t index) const {
        uint16_t value;
        getBytes(index, reinterpret_cast<uint8_t *>(&value), sizeof(value));
        return convertEndian(value);
    }

    uint32_t getInt() {
        uint32_t value;
        getBytes(reinterpret_cast<uint8_t *>(&value), sizeof(value));
        return convertEndian(value);
    }

    uint32_t getInt(uint32_t index) const {
        uint32_t value;
        getBytes(index, reinterpret_cast<uint8_t *>(&value), sizeof(value));
        return convertEndian(value);
    }

    uint64_t getLong() {
        uint64_t value;
        getBytes(reinterpret_cast<uint8_t *>(&value), sizeof(value));
        return convertEndian(value);
    }

    uint64_t getLong(uint32_t index) const {
        uint64_t value;
        getBytes(index, reinterpret_cast<uint8_t *>(&value), sizeof(value));
        return convertEndian(value);
    }

    float getFloat() {
        uint32_t intValue;
        getBytes(reinterpret_cast<uint8_t *>(&intValue), sizeof(intValue));
        intValue = convertEndian(intValue);
        return *reinterpret_cast<float *>(&intValue);
    }

    float getFloat(uint32_t index) const {
        uint32_t intValue;
        getBytes(index, reinterpret_cast<uint8_t *>(&intValue), sizeof(intValue));
        intValue = convertEndian(intValue);
        return *reinterpret_cast<float *>(&intValue);
    }

    double getDouble() {
        uint64_t longValue;
        getBytes(reinterpret_cast<uint8_t *>(&longValue), sizeof(longValue));
        longValue = convertEndian(longValue);
        return *reinterpret_cast<double *>(&longValue);
    }

    double getDouble(uint32_t index) const {
        uint64_t longValue;
        getBytes(index, reinterpret_cast<uint8_t *>(&longValue), sizeof(longValue));
        longValue = convertEndian(longValue);
        return *reinterpret_cast<double *>(&longValue);
    }

    std::vector<uint8_t> getRange(uint32_t start, uint32_t end) const {
        checkIndex(start);
        checkIndex(end - 1);
        if (start > end - 1) {
            throw std::out_of_range("Invalid range");
        }
        return std::vector<uint8_t>(buffer_.begin() + start, buffer_.begin() + end);
    }

    uint32_t getPosition() const {
        return position_;
    }

    void setPosition(uint32_t newPosition) {
        position_ = newPosition;
    }

    uint32_t getSize() const {
        return buffer_.size();
    }

private:
    uint8_t getByte(uint32_t index) const {
        return buffer_[index];
    }

    void checkIndex(uint32_t index) const {
        if (index >= buffer_.size()) {
            throw std::out_of_range("Index out of range");
        }
    }

    void checkRemaining(uint32_t length) const {
        if (position_ + length > buffer_.size()) {
            throw std::out_of_range("Insufficient remaining space in the buffer");
        }
    }

    template<typename T>
    T convertEndian(T value) const {
        if (endian_ == Endian::Big) {
            if (sizeof(T) == 2) {
                return ntohs(value);
            } else if (sizeof(T) == 4) {
                return ntohl(value);
            }
        }
        return value;
    }

    void clear() {
        buffer_.clear();
        position_ = 0;
    }

    std::vector<uint8_t> buffer_;
    uint32_t position_;
    Endian endian_;
};


#endif //AVBRIDGE_BYTEBUFFER_H
