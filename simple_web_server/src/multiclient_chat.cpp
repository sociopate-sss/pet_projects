#include "../include/multiclient_chat.hpp"

void MultiClientChat::OnClientConnected(int32_t client_socket) {
    std::string welcome_message = "Welcome to the Gachi Server\r\n";
    SendToClient(client_socket, welcome_message.c_str(), welcome_message.size());
}

void MultiClientChat::OnClientDataReceived(int32_t client_socket, const char* buf, size_t length) {
    SendToAllClients(client_socket, buf, length);
}