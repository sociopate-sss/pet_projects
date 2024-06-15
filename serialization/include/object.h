#pragma once

#include <cstdint>
#include <stdint.h>
#include <string>
#include <vector>

class Object {
protected:
    Object();
    Object(const std::string& name, uint8_t wrapper);

public:
    Object(Object&& other);

    virtual ~Object() = default;

    inline size_t GetSize() const { return size_; }
    inline std::string GetName() const { return name_; }
    inline uint8_t GetWrapper() const { return wrapper_; }

    virtual void serialize(std::vector<uint8_t>& buffer, size_t& iterator) const;

protected:
    std::string name_;
    uint32_t name_length_;
    uint8_t wrapper_;
    uint32_t size_;
};