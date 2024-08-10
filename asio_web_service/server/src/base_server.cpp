#include <exception>
#include <memory>

#include "../include/base_server.hpp"

template<typename T>
void net::BaseServer<T>::Start() {
    try {
        
        WaitConnections();

        server_thread_ = std::thread([this]() { context_.run(); });

    } catch (std::exception& e) {
        PrintServerError(static_cast<std::string>(e.what()));
    }
}

template<typename T>
void net::BaseServer<T>::Stop() {
    for (const std::shared_ptr<ServerConnection<T>>& conn : connections_) {
        conn->Disconnect();
    }

    context_.stop();

    if (server_thread_.joinable()) {
        server_thread_.join();
    }

    std::cout << "[SERVER] I'm out\n";

}

template<typename T>
void net::BaseServer<T>::WaitConnections() {

    acceptor_.async_accept(context_, [this](std::error_code ec, asio::ip::tcp::socket sock){
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
        std::make_shared<ServerConnection<T>>(std::move(sock), income_messages_, context_));

    connections_.back()->ConnectToClient(users_count_++);
}

template<typename T>
void net::BaseServer<T>::GetNewMessages() {
    while (income_messages_.size() > 0) {
        LinkedMessage<T> msg = income_messages_.front();
        income_messages_.pop();

        OnClientMessage(std::move(msg));
    }
}

template<typename T>
void net::BaseServer<T>::OnClientMessage(LinkedMessage<T>&& message) {
    SendToAllClients(std::move(message));
}

template<typename T>
void net::BaseServer<T>::SendToClient(const Message<T>& message, std::shared_ptr<ServerConnection<T>> client) {
    if (client != nullptr && client->IsConnected()) {
        client->SendMessage(message);
    } else {
        client.reset();
        ClearDisconnectedClients();
    }
}

template<typename T>
void net::BaseServer<T>::ClearDisconnectedClients() {
    connections_.erase(std::remove_if(begin(connections_), end(connections_), 
                            [](std::shared_ptr<ServerConnection<T>> conn){
                                return conn == nullptr;
                            }), end(connections_));
}

template<typename T>
void net::BaseServer<T>::SendToAllClients(LinkedMessage<T>&& message, bool ignore_sender) {

    bool has_disconnected = false;

    for (std::shared_ptr<ServerConnection<T>> conn : connections_) {
        if (conn != nullptr && conn->IsConnected()) {
            
            if (ignore_sender && conn == std::static_pointer_cast<Connection<T>>(message.connection)){
                continue;
            }

            conn->SendMessage(message.message);

        } else {
            has_disconnected = true;
            conn.reset();
        }
    }

    if (has_disconnected) {
        ClearDisconnectedClients();
    }
}


template class net::BaseServer<net::MessageTypes>;