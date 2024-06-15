#pragma once

#include <cstdint>
#include <memory>

#include "core.h"
#include "object.h"

class Array : public Object {
protected:
    Array(const std::string& name, uint8_t wrapper, uint8_t type, uint32_t length);

public:
    Array(Array&& other);

    ~Array() override = default;

    template<typename T>
    static std::unique_ptr<Array> create(const std::string& name, Core::Type type, const std::vector<T>& value) {
        std::unique_ptr<Array> array{
            std::make_unique<Array>(Array(name, static_cast<uint8_t>(Core::Wrapper::Array), 
                             static_cast<uint8_t>(type), static_cast<uint32_t>(value.size())))
        };
        array->data_ = std::make_unique<std::vector<uint8_t>>(std::vector<uint8_t>(sizeof(T) * value.size()));
        array->size_ += sizeof(T) * value.size();

        size_t iterator = 0;
        Core::encode<T>(*array->data_, iterator, value);

        return array;
    }

    void serialize(std::vector<uint8_t>& buffer, size_t& iterator) const override;

    inline size_t length() const { return length_; }

protected:
    uint8_t type_;
    uint32_t length_;
    std::unique_ptr<std::vector<uint8_t>> data_{nullptr};

};