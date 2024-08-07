#include <iostream>

#include "../include/connection.hpp"

template <typename T>
void net::Connection<T>::SendMessage(const Message<T>& message) {

    if (!IsConnected()) {
        std::cout << "[Connection #" << connection_id_ << "] Can't send message\n";
        return;
    }

    asio::post(context_, [&](){

        size_t queue_size = outcome_messages_.size();
        outcome_messages_.push(message);

        if (!queue_size) {
            WriteMessageHeader();
        }

    });
}

template <typename T>
void net::Connection<T>::WriteMessageHeader() {
    asio::async_write(socket_, asio::buffer(&outcome_messages_.front().message_header, sizeof(MessageHeader<T>)),
                            [&](asio::error_code e, size_t length){
                                if (!e) {
                                    WriteMessageBody();       
                                } else {
                                    std::cout << "[Connection #" << connection_id_ << "] Connection can't write MessageHeader\n";
                                    socket_.close();
                                }
                                
                            });
}

template <typename T>
void net::Connection<T>::WriteMessageBody() {
    asio::async_write(socket_, 
                    asio::buffer(outcome_messages_.front().message_body.data(), outcome_messages_.front().message_body.size()),
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


template<typename T>
void net::Connection<T>::ReadMessageHeader() {
    asio::async_read(socket_, asio::buffer(&temp_message_.message_header, sizeof(temp_message_.message_header)),
                    [&](asio::error_code ec, size_t length) {

                        if (!ec) {
                            ReadMessageBody();
                        } else {
                            std::cout << "[Connection #" << connection_id_ << "] Connection can't read MessageHeader\n";
                            socket_.close();
                        }

                    });
}

template<typename T>
void net::Connection<T>::ReadMessageBody() {
    asio::async_read(socket_, asio::buffer(temp_message_.message_body.data(), sizeof(temp_message_.message_body.size())),
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

template<typename T>
void net::Connection<T>::AddNewMessage() {
    temp_message_.connection_ = this->shared_from_this();
    income_messages_.push(std::move(temp_message_));
}

template class net::Connection<net::MessageTypes>;