#pragma once

#include <cstdint>
#include <memory>
#include <vector>

namespace net {

    enum class MessageTypes : uint16_t {
        Text
    };

    template<typename T>
    class Connection;

    template<typename T>
    struct MessageHeader {
        T Message_type;
        uint32_t message_size;
    };

    template<typename T>
    struct Message {
        MessageHeader<T> header;
        std::vector<uint8_t> body;
    };

    template<typename T>
    struct LinkedMessage {
        Message<T> message;
        std::shared_ptr<Connection<T>> connection = nullptr;
    };
    
}