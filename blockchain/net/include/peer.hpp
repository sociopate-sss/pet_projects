#pragma once

#include "connection.hpp"
#include "listener.hpp"
#include "p2p_message.hpp"

#include <excpt.h>
#include <memory>

namespace net {

    class Peer {
    public:
        Peer(uint16_t port, const std::string& endpoints_str) 
            : port_(port), listener_(context_, port_) {

            Start();
            ConnectToPeers(endpoints_str);
        }

        ~Peer() {
            for (size_t i = 0; i < connections_.size(); ++i) {
                if (connections_[i] != nullptr) {
                    connections_[i]->Disconnect();
                }
            }
        }

        void ConnectToPeers(const std::string& endpoints_str) {
            std::vector<asio::ip::tcp::endpoint> peers(std::move(EndpointsToVector(endpoints_str)));

            for(const asio::ip::tcp::endpoint& peer : peers) {
                asio::ip::tcp::socket socket(context_);

                ConnPtr conn = CreateConnection(std::move(socket));
                conn->ConnectToPeer(peer);
            }
        }

        void SendTransactionInfo(const std::string& transaction) {
            // TODO            
        }

        void Update() {
            while (listener_.GetConnectionsCount()) {
                asio::ip::tcp::socket& socket = listener_.GetNewConnection();
                if (socket.is_open()) {
                    CreateConnection(std::move(socket), true);
                }

                listener_.PopNewConnection();
            }

            while (income_messages_->size()) {
                LinkedP2PMessage message = income_messages_->front();
                income_messages_->pop();

                switch (message.GetType()) {
                    case P2PMessageType::ConnectRequest:
                        OnConnectRequest(std::move(message));
                        break;
                    case P2PMessageType::ConfirmConnect:
                        OnConfirmConnect(std::move(message));
                        break;
                    case P2PMessageType::TransactionInfo:
                        OnTransactionInfo(std::move(message));
                        break;
                }
            }
        }

    protected:
        void Start() {
            try {
                listener_.WaitConnections();
                context_thr_ = std::thread([this]() { context_.run(); });
            } catch(std::exception& e) {
                std::cout << e.what() << "\n";
            }
        }

        inline size_t GetIncomeSize() const { return income_messages_->size(); }

        void OnConnectRequest(LinkedP2PMessage&& message) {
            // TODO
        }

        void OnConfirmConnect(LinkedP2PMessage&& message) {
            // TODO
        }

        void OnTransactionInfo(LinkedP2PMessage&& message) {
            //TODO
        }

        void SendMessage(LinkedP2PMessage&& message) {
            if (message.connection_ != nullptr) {
                message.connection_->SendMessage(static_cast<P2PMessage>(message));
            }
        }

        ConnPtr CreateConnection(asio::ip::tcp::socket&& socket, bool start_reading = false) {
            ConnPtr conn = std::make_shared<Connection>(std::move(socket), context_, income_messages_, start_reading);
            conn->Setid(next_conn_id++);
            connections_.push_back(conn);

            return conn;
        }

    protected:
        uint16_t port_;
        size_t next_conn_id = 1;

        asio::io_context context_;
        std::thread context_thr_;

        Listener listener_;
        std::vector<ConnPtr> connections_;

        LinkedMessageQueue income_messages_;
    };

}