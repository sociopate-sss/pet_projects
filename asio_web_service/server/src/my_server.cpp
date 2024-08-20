#include "../include/my_server.hpp"

void net::MyServer::Update() {
    while (income_messages_.size() > 0) {
        LinkedMessage<MessageTypes> msg = income_messages_.front();
        income_messages_.pop();  

        message_handler_[msg.message.header.type](std::move(msg));
    }
}

void net::MyServer::SendToAllClients(LinkedMessage<MessageTypes>&& message) {

    bool has_disconnected = false;

    for (std::shared_ptr<ServerConnection<MessageTypes>> conn : connections_) {

        if (conn != nullptr && std::static_pointer_cast<Connection<MessageTypes>>(conn) != message.connection) {

            has_disconnected |= SendToClient(message.message, conn, message.connection->GetId());
            
        }
    }

    if (has_disconnected) {
        ClearDisconnectedClients();
    }
}

bool net::MyServer::SendToClient(const Message<MessageTypes>& message, std::shared_ptr<ServerConnection<MessageTypes>> client, uint32_t sender_id) {
    if (client->IsConnected()) {
        std::string sender_string = (sender_id == 0 ? "SERVER" : "Connection #" + std::to_string(sender_id));
        Message<MessageTypes> msg = MakeMessageToClient(message, sender_string);

        client->SendMessage(msg);
        
        return true;
    }

    return false;
}

net::Message<net::MessageTypes> net::MyServer::MakeMessageToClient(const Message<MessageTypes>& message, const std::string& sender) {

    Message<MessageTypes> result;

    result.header.type = MessageTypes::TextMessage;

    for (uint8_t elem : message.body) {
        result << elem;
    }

    result << static_cast<uint32_t>(message.header.size);

    for (char ch : sender) {
        result << ch;
    }

    result << static_cast<uint8_t>(sender.size());

    return result;
}