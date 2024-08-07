#include <exception>

#include "../include/base_server.hpp"

template<typename T>
void net::BaseServer<T>::Start() {
    try {
        
        WaitConnections();

        server_thread_ = std::thread([&]() { context_.run(); });

    } catch (std::exception& e) {
        PrintServerError(static_cast<std::string>(e.what()));
    }
}

template<typename T>
void net::BaseServer<T>::Stop() {
    for (ServerConnection<T>& conn : connections_) {
        conn.Disconnect();
    }
}

template<typename T>
void net::BaseServer<T>::WaitConnections() {

    acceptor_.async_accept(context_, [&](std::error_code ec, asio::ip::tcp::socket sock){
        if (!ec) {

            std::cout << "[SERVER] New connection: " << sock.remote_endpoint() << '\n';

            AddNewConnection(std::move(sock));

        } else {
            PrintServerError(ec.message());
        }

        WaitConnections();

    });

}

template<typename T>
void net::BaseServer<T>::AddNewConnection(asio::ip::tcp::socket&& sock) {
     connections_.push_back(
        ServerConnection<T>(std::move(sock), income_messages_, context_));

    connections_.back().ConnectToClient(users_count_++);
}

template class net::BaseServer<net::MessageTypes>;