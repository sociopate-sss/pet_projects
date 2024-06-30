#pragma once

#include <memory>
#include <string_view>
#include <vector>

#include "core_definitions.h"
#include "object.h"

namespace Core {

    //Utils functions

    void SerializeNSave(Object* obj);
    
    void Save(const std::string& file_name, const std::vector<uint8_t>& buffer);

    std::vector<uint8_t> Load(const std::string& file_name);

    uint8_t GetTypeSize(Type type);

    // Encoding(serializing) functions

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

    //Decoding(deserializing) functions

    template<typename T>
    inline T decode(const std::vector<uint8_t>& buffer, size_t& iterator) {
        T result = 0;
        T buf;

        for (size_t i = 0; i < sizeof(T); ++i) {    
            buf = buffer[iterator++] << (8 * (sizeof(T) - (i + 1)));
            result |= buf;
        }   

        return result;
    }

    // Float decoding
    template<>
    inline float decode<float>(const std::vector<uint8_t>& buffer, size_t& iterator) {
        int32_t value = decode<int64_t>(buffer, iterator);
        return *reinterpret_cast<float*>(&value);
    }

    // Double decoding
    template<>
    inline double decode<double>(const std::vector<uint8_t>& buffer, size_t& iterator) {
        int64_t value = decode<int64_t>(buffer, iterator);
        return *reinterpret_cast<double*>(&value);
    }

    //Vector decoding
    template<typename T>
    inline void decode(const std::vector<uint8_t>&buffer, size_t& iterator, std::vector<T>& value) {
        for (size_t i = 0; i < value.size(); ++i) {
            value[i] = buffer[iterator++];
        }
    }
   
    // String decoding
    // Here we used that the string itself is preceded by its size
    template<>
    inline std::string decode<std::string>(const std::vector<uint8_t>&buffer, size_t& iterator) {
        iterator -= 4;
        uint32_t length = decode<uint32_t>(buffer, iterator);

        std::string result(buffer.begin() + iterator, buffer.begin() + iterator + length);
        iterator += length;
        
        return result;
    }

}