#pragma once

#include <memory>
#include <string_view>
#include <vector>

#include "core_definitions.h"
#include "object.h"

namespace Core {

    //Utils functions

    void GetTypeSize(Core::Type);

    void SerializeNSave(Object* obj);
    
    void Save(std::string file_name, const std::vector<uint8_t>& buffer);

    // Encoding functions

    // Integer types encoding
    template<typename T>
    inline void encode(std::vector<uint8_t>& buffer, size_t& iterator, T value) {
        for (size_t i = 0; i < sizeof(T); ++i) {    
            buffer[iterator++] = value >> (8 * (sizeof(T) - (i + 1)));
        }   
    }

    // Float encoding
    template<>
    inline void encode<float>(std::vector<uint8_t>& buffer, size_t& iterator, float value) {
        encode<int32_t>(buffer, iterator, *reinterpret_cast<int32_t*>(&value));
    }

    // Double encoding
    template<>
    inline void encode<double>(std::vector<uint8_t>& buffer, size_t& iterator, double value) {
        encode<int64_t>(buffer, iterator, *reinterpret_cast<int64_t*>(&value));
    }

    // Vector encoding
    template<typename T>
    inline void encode(std::vector<uint8_t>&buffer, size_t& iterator, const std::vector<T>& value) {
        for (auto element : value) {
            encode<T>(buffer, iterator, element);
        }
    }
   
    // String encoding
    template<>
    inline void encode<std::string_view>(std::vector<uint8_t>&buffer, size_t& iterator, std::string_view value) {
        for (auto element : value) {
            encode<uint8_t>(buffer, iterator, element);
        }
    }

}