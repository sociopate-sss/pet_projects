#pragma once

#include "base_client.hpp"

namespace net {

    class MyClient : public net::BaseClient<net::MessageTypes> {
    public:
        MyClient() {}

        ~MyClient() override {
            Disconnect();
        }

        void Update() override;
    
    protected:
        void OnTextMessage(LinkedMessage<MessageTypes>&& message);

    protected:
        MessageHandler message_handler_ = {
            {
                MessageTypes::TextMessage,
                [this](LinkedMessage<MessageTypes>&& message) { 
                    OnTextMessage(std::move(message)); 
                }
            }
        };
    };

}