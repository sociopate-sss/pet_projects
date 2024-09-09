#pragma once

#define ASIO_STANDALONE

#ifdef _WIN32
    #define _WIN32_WINNT 0x0A00
#endif

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include "../../utils/include/thread_safe_queue.hpp"

#include "p2p_message.hpp"
#include "net_utils.hpp"

#include <iostream>

namespace net {

    class Connection : public std::enable_shared_from_this<Connection> {
    public:
        Connection(asio::ip::tcp::socket&& sock, asio::io_context& context, LinkedMessageQueue income_messages, bool start_reading = false) 
                    : socket_(std::move(sock)), context_(context), income_messages_(income_messages) {

            if (start_reading) {
                ReadMessageHeader();
            }
        } 

        ~Connection() {
            Disconnect();
        }

        inline bool IsConnected() const { return socket_.is_open(); }

        void Disconnect() {
            socket_.close();
        }

        void ConnectToPeer(const asio::ip::tcp::endpoint& peer) {
            asio::async_connect(socket_, peer, [this](const asio::error_code& ec){
                if (!ec) {  
                    ReadMessageHeader();
                }
            });
        }

        void SendMessage(P2PMessage&& message) {

            if (!IsConnected()) {
                std::cout << "Connection closed\n";
                return;
            }

            asio::post(context_, [this, message](){
                size_t prev_size = outcome_messages_->size();
                outcome_messages_->push(std::move(message));

                if (!prev_size) {
                    WriteMessageHeader();
                }
            });

        }

        void Setid(size_t new_id) {
            id_ = new_id;
        }

        size_t GetId() const { return id_; }

    protected:
        void ReadMessageHeader() {
            asio::async_read(socket_, 
                            asio::buffer(&temp_message_header_, sizeof(temp_message_header_)), 
                            [this](asio::error_code& ec, size_t bytes_count){
                                if(!ec) {
                                    if (temp_message_header_.size > 0) {
                                        ReadRequestBody(temp_message_header_.size);
                                    } else {
                                        ReadMessageHeader();
                                    }
                                } else {
                                    std::cout << "Can't read message header: ";
                                    std::cout << ec.message() << '\n';
                                    Disconnect();
                                }
                            });
        }

        void ReadRequestBody(size_t bytes_count) {
            asio::async_read(socket_, asio::buffer(temp_message_body_), 
                            asio::transfer_exactly(bytes_count),
                            [this](asio::error_code& ec, size_t bytes_recv) {
                                if(!ec) {

                                    AddIncomeMessage();
                                    
                                    ReadMessageHeader();

                                } else {
                                    std::cout << "Can't read message body: ";
                                    std::cout << ec.message() << '\n';
                                    Disconnect();
                                }
                            });
        }

        void WriteMessageHeader() {
            asio::async_write(socket_, asio::buffer(
                            &outcome_messages_->front().header, sizeof(outcome_messages_->front().header)),
                            [this](asio::error_code& ec, size_t bytes_count) {
                                if (!ec) {
                                    if (outcome_messages_->front().header.size > 0) {
                                        WriteMessageBody();
                                    } else {
                                        outcome_messages_->pop();
                                        WriteMessageHeader();
                                    }
                                } else {
                                    std::cout << "Can't write message header: ";
                                    std::cout << ec.message() << '\n';
                                    Disconnect();
                                }
                            });
        }

        void WriteMessageBody() {
            std::string message = outcome_messages_->front().body.dump();

            asio::async_write(socket_, asio::buffer(message),
                            asio::transfer_exactly(message.size()),
                            [this](asio::error_code& ec, size_t bytes_count){
                                if (!ec) {
                                    outcome_messages_->pop();

                                    if (outcome_messages_->size()) {
                                        WriteMessageHeader();
                                    }
                                } else {
                                    std::cout << "Can't write message body: ";
                                    std::cout << ec.message() << '\n';
                                    Disconnect();
                                }
                            });
        }

        void AddIncomeMessage() {
            LinkedP2PMessage message;
            message.header = temp_message_header_;
            message.body = json::parse(temp_message_body_);
            message.connection_ = shared_from_this();
            
            income_messages_->push(std::move(message));
        }

    protected:
        size_t id_ = 0;

        asio::ip::tcp::socket socket_;
        asio::io_context& context_;

        P2PMessageHeader temp_message_header_;
        std::string temp_message_body_;

        LinkedMessageQueue income_messages_;
        MessageQueue outcome_messages_;
    };

}