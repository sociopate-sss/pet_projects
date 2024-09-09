#pragma once

#define ASIO_STANDALONE

#ifdef _WIN32
    #define _WIN32_WINNT 0x0A00
#endif

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include <iostream>

#include "../../utils/include/thread_safe_queue.hpp"

namespace net {
    
    class Listener {
    public:
        Listener(asio::io_context& context, uint16_t port)
                : 
                context_(context), 
                port_(port),
                acceptor_(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port_)) {}

        ~Listener() {
            acceptor_.cancel();

            while (new_connections_.size()) {
                asio::ip::tcp::socket& socket = new_connections_.front();

                if (socket.is_open()) {
                    socket.close();
                }

                new_connections_.pop();
            }
        }

        void WaitConnections() {
            acceptor_.async_accept([this](asio::error_code ec, asio::ip::tcp::socket sock){
                if (!ec) {
                    new_connections_.push(std::move(sock));

                    std::cout << "New connected peer: " << sock.remote_endpoint() << '\n';
                } else {
                    setlocale(LC_ALL, "ru");
                    std::cout << ec.message() << '\n';
                }

                WaitConnections();
            });
        }

        size_t GetConnectionsCount() const { return new_connections_.size(); }
        
        asio::ip::tcp::socket& GetNewConnection() { return new_connections_.front(); }

        void PopNewConnection() { new_connections_.pop(); }

    protected:
        uint16_t port_;
        uint64_t next_conn_id_ = 0;

        ThreadSafeQueue<asio::ip::tcp::socket> new_connections_;

        asio::ip::tcp::acceptor acceptor_;
        asio::io_context& context_;
    };

}