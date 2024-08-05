#pragma once

#include <memory>

#include "common.hpp"
#include "message.hpp"
#include "thread_safe_queue.hpp"

namespace net {

    template<typename T>
    class Connection : public std::enable_shared_from_this<Connection<T>> {
    public:
        Connection (asio::ip::tcp::socket socket, ThreadSafeQueue<Message<T>>& income_messages, asio::io_context& context) 
                    : socket_(std::move(socket)),  income_messages_(income_messages), context_(context) {}

        ~Connection () {
            Disconnect();
        }
        
        void Disconnect () {
            asio::post([&]() {
                socket_.close();
            });
        }

        bool IsConnected() const {
            return socket_.is_open();
        }
        
        uint32_t GetId() const {
            return connection_id_;
        }

        void SetId(uint32_t id) {
            connection_id_ = id;
        }

        void SendMessage(const Message<T>& message);

    protected:
        // It is guaranteed that outcome_message_ and MessageBody of the messages aren't empty 
        void WriteMessageHeader();

        void WriteMessageBody();

        // It is guaranteed that income_message_ and MessageBody of the messages aren't empty
        void ReadMessageHeader();

        void ReadMessageBody();

        void AddNewMessage();

    protected:
        asio::io_context& context_;
        asio::ip::tcp::socket socket_;

        ThreadSafeQueue<Message<T>> outcome_messages_;
        ThreadSafeQueue<Message<T>>& income_messages_;

        uint32_t connection_id_;

        Message<T> temp_message_;
    };

}