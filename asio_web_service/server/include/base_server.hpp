#pragma once

#include <iostream>

#include "../../connection/include/server_connection.hpp"

namespace net {

    template<typename T>
    class BaseServer {
    public:
        BaseServer(uint16_t port) : port_(port), acceptor_(context_, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port_)) {}

        virtual ~BaseServer() {
            Stop();
        }

        void Start() {
            try {
                
                WaitConnections();

                server_thread_ = std::thread([this]() { context_.run(); });

            } catch (std::exception& e) {
                PrintServerError(static_cast<std::string>(e.what()));
            }
        }

        void Stop() {
            for (const std::shared_ptr<ServerConnection<T>>& conn : connections_) {
                conn->Disconnect();
            }

            context_.stop();

            if (server_thread_.joinable()) {
                server_thread_.join();
            }

            std::cout << "[SERVER] I'm out\n";

        }

        virtual void Update() = 0;

    protected:
        void PrintServerError(const std::string& error_message) const { std::cout << "[SERVER ERROR] " << error_message << '\n'; }

        void WaitConnections() {

            acceptor_.async_accept([this](std::error_code ec, asio::ip::tcp::socket sock){
                if (!ec) {

                    std::cout << "[SERVER] New connection: " << sock.remote_endpoint() << '\n';

                    AddNewConnection(std::move(sock));

                } else {
                    PrintServerError(ec.message());
                }

                WaitConnections();

            });

        }

        void AddNewConnection(asio::ip::tcp::socket&& sock) {
            connections_.push_back(
                std::make_shared<ServerConnection<T>>(std::move(sock), income_messages_, context_));

            users_count_++;
            connections_.back()->ConnectToClient(user_id++);
        }

        void OnClientConnect(std::shared_ptr<ServerConnection<T>> client);

        void OnClientDisconnect(std::shared_ptr<ServerConnection<T>> client) {
            std::cout << "[Connection #" << client->GetId() << "] Disconnected\n";
            --users_count_;
        }

        virtual bool SendToClient(const Message<T>& message, std::shared_ptr<ServerConnection<T>> client, uint32_t sender_id = 0) = 0;

        virtual void SendToAllClients(LinkedMessage<T>&& message) = 0;

        void ClearDisconnectedClients() {
            connections_.erase(std::remove_if(begin(connections_), end(connections_), 
                                    [this](std::shared_ptr<ServerConnection<T>> conn){
                                        if (conn == nullptr || !conn->IsConnected()) {
                                            OnClientDisconnect(conn);
                                            return true;   
                                        }

                                        return false;

                                    }), end(connections_));
        }

        inline uint32_t UsersCount() const { return users_count_; }

    protected:
        uint16_t port_;
        uint32_t users_count_ = 0;
        uint32_t user_id = 1;

        asio::io_context context_;
        asio::ip::tcp::acceptor acceptor_;

        std::vector<std::shared_ptr<ServerConnection<T>>> connections_;
        ThreadSafeQueue<LinkedMessage<T>> income_messages_;

        std::thread server_thread_;

    };

}