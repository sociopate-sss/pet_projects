#pragma once

#include <iostream>
#include <memory>

#include "../../utils/common.hpp"
#include "../../utils/message.hpp"
#include "../../utils/thread_safe_queue.hpp"

namespace net {

    template<typename T>
    class Connection : public std::enable_shared_from_this<Connection<T>> {
    public:
        Connection (asio::ip::tcp::socket&& socket, ThreadSafeQueue<LinkedMessage<T>>& income_messages, asio::io_context& context) 
                    : socket_(std::move(socket)),  income_messages_(income_messages), context_(context) {}

        ~Connection () {
            Disconnect();
        }
        
        void Disconnect () {
            asio::post([&]() {
                socket_.close();
            });
        }

        bool IsConnected() const {
            return socket_.is_open();
        }
        
        uint32_t GetId() const {
            return connection_id_;
        }

        void SetId(uint32_t id) {
            connection_id_ = id;
        }

        void SendMessage(const Message<T>& message) {

            if (!IsConnected()) {
                std::cout << "[Connection #" << connection_id_ << "] Can't send message\n";
                return;
            }

            asio::post(context_, [&](){

                size_t queue_size = outcome_messages_.size();
                outcome_messages_.push({message, this->shared_from_this()});

                if (!queue_size) {
                    WriteMessageHeader();
                }

            });
        }

    protected:
        // It is guaranteed that outcome_message_ and MessageBody of the messages aren't empty 
        void WriteMessageHeader() {
            asio::async_write(socket_, asio::buffer(&outcome_messages_.front().message.header, sizeof(MessageHeader<T>)),
                                    [&](asio::error_code e, size_t length){
                                        if (!e) {
                                            WriteMessageBody();       
                                        } else {
                                            std::cout << "[Connection #" << connection_id_ << "] Connection can't write MessageHeader\n";
                                            socket_.close();
                                        }
                                        
                                    });
        }

        void WriteMessageBody() {
            asio::async_write(socket_, 
                            asio::buffer(outcome_messages_.front().message.body.data(), outcome_messages_.front().message.body.size()),
                            [&](asio::error_code e, size_t length) {
                                if (!e) {
                                    
                                    outcome_messages_.pop();

                                    if (outcome_messages_.size()) {
                                        WriteMessageHeader();
                                    }

                                } else {
                                    std::cout << "[Connection #" << connection_id_ << "] Connection can't write MessageBody\n";
                                    socket_.close();
                                }
                            });
        }

        // It is guaranteed that income_message_ and MessageBody of the messages aren't empty
        void ReadMessageHeader() {
            asio::async_read(socket_, asio::buffer(&temp_message_.message.header, sizeof(temp_message_.message.header)),
                            [&](asio::error_code ec, size_t length) {

                                if (!ec) {
                                    ReadMessageBody();
                                } else {
                                    std::cout << "[Connection #" << connection_id_ << "] Connection can't read MessageHeader\n";
                                    socket_.close();
                                }

                            });
        }

        void ReadMessageBody() {
            asio::async_read(socket_, asio::buffer(temp_message_.message.body.data(), sizeof(temp_message_.message.body.size())),
                            [&](asio::error_code ec, size_t length) {

                                if (!ec) {

                                    AddNewMessage();    
                                    
                                    ReadMessageHeader();
                                } else {
                                    std::cout << "[Connection #" << connection_id_ << "] Connection can't read MessageHeader\n";
                                    socket_.close();
                                }

                            });
        }

        void AddNewMessage(){
            temp_message_.connection = std::static_pointer_cast<Connection<T>>(this->shared_from_this());
            income_messages_.push(std::move(temp_message_));
        }

    protected:
        asio::io_context& context_;
        asio::ip::tcp::socket socket_;

        ThreadSafeQueue<LinkedMessage<T>> outcome_messages_;
        ThreadSafeQueue<LinkedMessage<T>>& income_messages_;

        uint32_t connection_id_;

        LinkedMessage<T> temp_message_;
    };

}