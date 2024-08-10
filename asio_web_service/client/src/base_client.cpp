#include <iostream>
#include <memory>

#include "../include/base_client.hpp"

template<typename T>
void net::BaseClient<T>::ConnectToServer(const std::string& host, uint16_t port) {

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

template<typename T>
void net::BaseClient<T>::Disconnect() {
    if (connection_) {
        connection_->Disconnect();
    }

    context_.stop();

    if (client_thread_.joinable()) {
        client_thread_.join();
    }
}


template<typename T>
void net::BaseClient<T>::Start() {
    client_thread_ = std::thread([&]() { context_.run(); });
}

template<typename T>
void net::BaseClient<T>::SendToServer(const Message<T>& message) {
    if (connection_) {
        connection_->SendMessage(message);
    }
}

template class net::BaseClient<net::MessageTypes>;