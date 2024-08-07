#pragma once

#include <iostream>
#include <vector>

#include "common.hpp"
#include "message.hpp"
#include "server_connection.hpp"
#include "thread_safe_queue.hpp"

namespace net {

    template<typename T>
    class BaseServer {
    public:
        BaseServer(uint16_t port) : port_(port), acceptor_(context_, asio::ip::tcp::v4(), port_) {}

        ~BaseServer() {
            Stop();
        }

        void Start();

        void Stop();

    protected:
        void PrintServerError(const std::string& error_message) const { std::cout << "[SERVER ERROR] " << error_message << '\n'; }

        void WaitConnections();

        void AddNewConnection(asio::ip::tcp::socket&& sock);

    protected:
        uint16_t port_;
        uint32_t users_count_ = 0;

        asio::io_context context_;
        asio::ip::tcp::acceptor acceptor_;

        std::vector<ServerConnection<T>> connections_;
        ThreadSafeQueue<Message<T>> income_messages_;

        std::thread server_thread_;

    };

}