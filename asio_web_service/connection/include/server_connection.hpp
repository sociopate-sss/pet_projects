#pragma once

#include "connection.hpp"

namespace net {

    template<typename T>
    class ServerConnection : public Connection<T> {
    public:
        ServerConnection (asio::ip::tcp::socket&& socket, ThreadSafeQueue<LinkedMessage<T>>& income_messages, asio::io_context& context) 
                    : Connection<T>(std::move(socket), income_messages, context) {}

        ~ServerConnection () {
            this->Disconnect();
        }
    
        void ConnectToClient(uint32_t id) {
            if (this->IsConnected()) {
                this->SetId(id);
                this->ReadMessageHeader();
            }
        }

    };

}