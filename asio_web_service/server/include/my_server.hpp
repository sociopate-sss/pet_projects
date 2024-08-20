#pragma once

#include "base_server.hpp"

namespace net {

    class MyServer : public BaseServer<MessageTypes> {
    public:

        MyServer(uint16_t port) : BaseServer(port) {}

        ~MyServer() override {
            Stop();
        }

        void Update() override;

    protected: 
        void SendToAllClients(LinkedMessage<MessageTypes>&& message) override;

        bool SendToClient(const Message<MessageTypes>& message, std::shared_ptr<ServerConnection<MessageTypes>> client, uint32_t sender_id = 0) override;

        Message<MessageTypes> MakeMessageToClient(const Message<MessageTypes>& message, const std::string& sender);

    private:
        MessageHandler message_handler_ = {
            {
                MessageTypes::TextMessage, 
                [this](LinkedMessage<MessageTypes>&& message) { 
                    SendToAllClients(std::move(message)); 
                }
            }
        };

    };

}