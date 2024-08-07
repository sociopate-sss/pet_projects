#pragma once

#include <memory>
#include <thread>

#include "common.hpp"
#include "client_connection.hpp"
#include "message.hpp"
#include "thread_safe_queue.hpp"

namespace net {
    
    template <typename T>
    class BaseClient {
    public:
        BaseClient() {}

        ~BaseClient() {
            Disconnect();
        }

        bool IsConnected() { return (connection_ ? connection_->IsConnected() : false); }

        void Disconnect();

        void ConnectToServer(const std::string& host, uint16_t port);

        void SendToServer(const Message<T>& message);

    protected:
        void Start();

    protected:
        asio::io_context context_;
        std::unique_ptr<net::ClientConnection<T>> connection_;
        std::thread client_thread_;

        ThreadSafeQueue<Message<T>> income_messages_;
    };

}