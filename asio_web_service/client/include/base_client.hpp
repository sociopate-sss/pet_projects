#pragma once

#include <thread>

#include "../../connection/include/client_connection.hpp"

namespace net {
    
    template <typename T>
    class BaseClient {
    public:
        BaseClient() {}

        virtual ~BaseClient() {
            Disconnect();
        }

        bool IsConnected() { return (connection_ ? connection_->IsConnected() : false); }

        void Disconnect() {
            if (connection_) {
                connection_->Disconnect();
            }

            context_.stop();

            if (client_thread_.joinable()) {
                client_thread_.join();
            }
        }

        void ConnectToServer(const std::string& host, uint16_t port) {

            try {

                asio::ip::tcp::resolver resolver(context_);
                asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

                asio::ip::tcp::socket sock(context_);
                connection_ = std::make_shared<ClientConnection<T>>(
                                            std::move(sock), 
                                            income_messages_, 
                                            context_);

                connection_->ConnectToServer(endpoints);

                Start();

            } catch (std::exception& e) {
                std::cout << "[CLIENT ERROR!!! While trying connect to " << host << ":" << port << "] " << e.what() << "\n";
            }

        }

        void SendToServer(const Message<T>& message) {
            if (connection_) {
                connection_->SendMessage(message);
            }
        }

        virtual void Update() = 0;

    protected:
        void Start() {
            client_thread_ = std::thread([&]() { context_.run(); });
        }

    protected:
        asio::io_context context_;
        std::shared_ptr<net::ClientConnection<T>> connection_;
        std::thread client_thread_;

        ThreadSafeQueue<LinkedMessage<T>> income_messages_;

    };

}