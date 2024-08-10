#pragma once

#include "connection.hpp"

namespace net {

    template<typename T>
    class ClientConnection : public Connection<T> {
    public:
        ClientConnection (asio::ip::tcp::socket&& socket, ThreadSafeQueue<LinkedMessage<T>>& income_messages, asio::io_context& context) 
                    : Connection<T>(std::move(socket), income_messages, context) {}

        ~ClientConnection () {
            this->Disconnect();
        }
    
        void ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints) {
            asio::async_connect(this->socket_, endpoints, [&](asio::error_code ec, const asio::ip::tcp::endpoint&){
                if (!ec) {
                    this->ReadMessageHeader();
                }                    
            });
        }

    };

}