#pragma once

#include "tcp_server.hpp"

class MultiClientChat : public TCPServer {
public:
    MultiClientChat(std::string ip_address, uint16_t port)
                    : TCPServer(ip_address, port) {}

protected:
    void OnClientConnected(int32_t client_socket) override;

    void OnClientDataReceived(int32_t client_socket, const char* buf, size_t length) override;
};