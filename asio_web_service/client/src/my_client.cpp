#include "../include/my_client.hpp"

void net::MyClient::Update() {
    while (income_messages_.size() > 0) {
        LinkedMessage<MessageTypes> msg = income_messages_.front();
        income_messages_.pop();

        std::cout << "Get new message!!\n";

        message_handler_[msg.message.header.type](std::move(msg));
    }
}

void net::MyClient::OnTextMessage(LinkedMessage<MessageTypes>&& message) {

    uint8_t name_len;
    message.message >> name_len;

    std::string sender_name;
    sender_name.resize(name_len, '.');
    for (char& ch : sender_name) {
        message.message >> ch;
    }
    std::reverse(sender_name.begin(), sender_name.end());

    uint32_t str_len;
    message.message >> str_len;
   
    std::string str;
    str.resize(str_len);
    for (char& ch : str) {
        message.message >> ch;
    }
    std::reverse(str.begin(), str.end());

    std::cout << "[" << sender_name << "]: " << str << '\n';
}