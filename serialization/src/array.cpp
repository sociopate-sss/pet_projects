#include <cstdint>
#include <memory>

#include "../include/array.h"
#include "../include/core.h"

Array::Array(Array&& other) : Object(std::move(other)) {
    std::swap(length_, other.length_);
    std::swap(type_, other.type_);
    std::swap(data_, other.data_);
}

Array::Array(const std::string& name, uint8_t wrapper, uint8_t type, uint32_t length) 
        : Object(name, wrapper), type_(type), length_(length) {
            size_ += sizeof(type) + sizeof(length_);
        }

void Array::serialize(std::vector<uint8_t>& buffer, size_t& iterator) const {
    Object::serialize(buffer, iterator);
    Core::encode<uint8_t>(buffer, iterator, type_);
    Core::encode<uint32_t>(buffer, iterator, length_);
    Core::encode<uint8_t>(buffer, iterator, *data_);
}