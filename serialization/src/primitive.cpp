#include "../include/primitive.h"

#include <stdint.h>

Primitive::Primitive(Primitive&& other) : Object(std::move(other)) {
    std::swap(type_, other.type_);
    std::swap(data_, other.data_);
}

Primitive::Primitive (const std::string& name, uint8_t wrapper, uint8_t type) 
            : 
            Object(name, wrapper), type_(type) {
                size_ += sizeof(type_);
            }

void Primitive::serialize(std::vector<uint8_t>& buffer, size_t& iterator) const {
    Object::serialize(buffer, iterator);
    Core::encode<uint8_t>(buffer, iterator, type_);
    Core::encode<uint8_t>(buffer, iterator, *data_);
}