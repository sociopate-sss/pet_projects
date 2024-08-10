#pragma once

#include <iostream>

#include "../../connection/include/server_connection.hpp"

namespace net {

    template<typename T>
    class BaseServer {
    public:
        BaseServer(uint16_t port) : port_(port), acceptor_(context_, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port_)) {}

        ~BaseServer() {
            Stop();
        }

        void Start();

        void Stop();

    protected:
        void PrintServerError(const std::string& error_message) const { std::cout << "[SERVER ERROR] " << error_message << '\n'; }

        void WaitConnections();

        void AddNewConnection(asio::ip::tcp::socket&& sock);

        void GetNewMessages();

        void OnClientMessage(LinkedMessage<T>&& message);

        void SendToClient(const Message<T>& message, std::shared_ptr<ServerConnection<T>> client);

        void SendToAllClients(LinkedMessage<T>&& message, bool ignore_sender = true);

        void ClearDisconnectedClients();

    protected:
        uint16_t port_;
        uint32_t users_count_ = 0;

        asio::io_context context_;
        asio::ip::tcp::acceptor acceptor_;

        std::vector<std::shared_ptr<ServerConnection<T>>> connections_;
        ThreadSafeQueue<LinkedMessage<T>> income_messages_;

        std::thread server_thread_;

    };

}