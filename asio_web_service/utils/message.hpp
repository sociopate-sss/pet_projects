#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

namespace net {

    enum class MessageTypes : uint16_t {
        TextMessage = 0,
        CheckPing
    };

    template<typename T>
    class Connection;

    template<typename T>
    struct MessageHeader {
        T type;
        uint32_t size = 0;
    };

    template<typename T>
    struct Message {
        MessageHeader<T> header{};
        std::vector<uint8_t> body{};

        template<typename U>
        friend Message<T>& operator <<(Message<T>& message, const U& data) {

            static_assert(std::is_standard_layout<U>::value, "The data is too complex to output to the stream\n");

            size_t prev_size = static_cast<size_t>(message.body.size());

            message.body.resize(message.body.size() + sizeof(data));
            memcpy(message.body.data() + prev_size, &data, sizeof(data));

            message.header.size += sizeof(data);

            return message;
        }

        template<typename U>
        friend Message<T>& operator >>(Message<T>& message, U& data) {

            static_assert(std::is_standard_layout_v<U>, "The data is too complex to output to the stream");

            size_t new_size = message.body.size() - sizeof(data);

            memcpy(&data, message.body.data() + new_size, sizeof(data));

            message.body.resize(new_size);
            message.header.size = new_size;

            return message;
        }

    };

    template<typename T>
    struct LinkedMessage {
        Message<T> message;
        std::shared_ptr<Connection<T>> connection = nullptr;
    };  

    using message_handler_func = std::function<void(LinkedMessage<MessageTypes>&&)>;
    using MessageHandler = std::unordered_map<MessageTypes, message_handler_func>;
    
}