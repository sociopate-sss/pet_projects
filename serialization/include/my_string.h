#pragma once

#include "array.h"

#include <memory>

class String : public Array {
private:
    String(const std::string& name, uint8_t wrapper, uint8_t type, uint32_t length)
          : Array(name, wrapper, type, length) {}

public:
    String(String&& other) : Array(std::move(other)) {}

    ~String() override = default;

    template<typename T>
    static std::unique_ptr<String> create(const std::string& name, Core::Type type, T value) {

        std::string_view sv(value);

        std::unique_ptr<String> str{
            std::make_unique<String>(String(name, static_cast<uint8_t>(Core::Wrapper::String), 
                             static_cast<uint8_t>(type), static_cast<uint32_t>(sv.size())))
        };
        str->data_ = std::make_unique<std::vector<uint8_t>>(std::vector<uint8_t>(sv.size()));
        str->size_ += sv.size();

        size_t iterator = 0;
        Core::encode<std::string_view>(*str->data_, iterator, sv);

        return str;
    }
};