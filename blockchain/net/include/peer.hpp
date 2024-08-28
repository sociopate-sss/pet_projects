#pragma once

#include "connection.hpp"
#include "p2p_message.hpp"

#include <memory>

namespace net {

    class Peer {
    public:
        Peer(uint16_t port) 
            : 
            port_(port), 
            acceptor_(context_, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {}

        void ConnectToPeers(const std::string& str) {
            std::vector<asio::ip::tcp::endpoint> peers(std::move(EndpointsToVector(str)));

            for(const asio::ip::tcp::endpoint& peer : peers) {
                asio::ip::tcp::socket sock(context_);
                Conn_ptr conn = std::make_unique<Connection>(std::move(sock), context_, income_messages_);
                
                conn->ConnectToPeer(peer);
                connections_.push_back(std::move(conn));
            }
        }

        void Update() {
            while(income_messages_->size()) {
                P2PMessage message = income_messages_->front();
                income_messages_->pop();

                message_handler_[message.header.type](std::move(message));
            }
        }

    protected:
        inline size_t GetIncomeSize() const { return income_messages_->size(); }

    protected:
        uint16_t port_;

        std::vector<Conn_ptr> connections_;

        asio::io_context context_;
        asio::ip::tcp::acceptor acceptor_;

        TSQ_sh_ptr income_messages_;
        MessageHandler message_handler_;
    };

}