#include <string_view>

#include "../include/core.h"
#include "../include/object.h"

Object::Object(Object&& other){
    std::swap(name_, other.name_);
    std::swap(name_length_, other.name_length_);
    std::swap(wrapper_, other.wrapper_);
    std::swap(size_, other.size_);
}

Object::Object() 
        : 
        name_("none"), wrapper_(static_cast<uint8_t>(Core::Wrapper::Object)), name_length_(static_cast<uint32_t>(name_.size())),
        size_(name_length_ + sizeof(size_) + sizeof(wrapper_) + sizeof(name_length_)) {}

Object::Object(const std::string& name, uint8_t wrapper) 
        : 
        name_(name), wrapper_(wrapper), name_length_(static_cast<uint32_t>(name_.size())),
        size_(name_length_ + sizeof(size_) + sizeof(wrapper_) + sizeof(name_length_)) {}

void Object::serialize(std::vector<uint8_t>& buffer, size_t& iterator) const {
    Core::encode<uint32_t>(buffer, iterator, size_);
    Core::encode<uint32_t>(buffer, iterator, name_length_);
    Core::encode<std::string_view>(buffer, iterator, std::string_view(name_));
    Core::encode<uint8_t>(buffer, iterator, wrapper_);
}