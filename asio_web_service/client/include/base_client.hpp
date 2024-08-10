#pragma once

#include <thread>

#include "../../connection/include/client_connection.hpp"

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
        std::shared_ptr<net::ClientConnection<T>> connection_;
        std::thread client_thread_;

        ThreadSafeQueue<LinkedMessage<T>> income_messages_;
    };

}