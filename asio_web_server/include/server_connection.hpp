#pragma once

#include "connection.hpp"

namespace net {

    template<typename T>
    class ServerConnection : public Connection<T> {
    public:
        ServerConnection (asio::ip::tcp::socket&& socket, ThreadSafeQueue<Message<T>>& income_messages, asio::io_context& context) 
                    : Connection<T>(std::move(socket), income_messages, context) {}

        ~ServerConnection () {
            Disconnect();
        }
    
        void ConnectToClient(uint32_t id) {
            if (IsConnected()) {
                SetId(id);
                ReadMessageHeader();
            }
        }

    };

}