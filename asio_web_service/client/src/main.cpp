
#include "../include/my_client.hpp"

#include <chrono>
#include <thread>

int main() {
    net::MyClient client;
    client.ConnectToServer("192.168.1.8", 56000);

    std::thread writing = std::thread([&]() {
        
        std::string text;

        while(true) {
            std::cout << "Your message: ";
            getline(std::cin, text);
            if (text.size() == 0) {
                continue;
            }

            net::Message<net::MessageTypes> message;
            message.header.type = net::MessageTypes::TextMessage;
            for (const char& ch : text) {
                message << ch;
            }
            client.SendToServer(message);

            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        }
    });

    bool is_quit = false;

    while(!is_quit) {
        client.Update();        
    }

    writing.join();

    return 0;
}