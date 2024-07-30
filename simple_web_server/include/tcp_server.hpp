#pragma once

#include <string>

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

class TCPServer {
public:
    TCPServer(std::string ip_address, uint16_t port) 
            : ip_address_(ip_address), port_(port) {}

    uint32_t init();

    uint32_t run();

protected:
    virtual void OnClientConnected(int32_t client_socket){}

    virtual void OnClientDisconnected(int32_t client_socket){}

    virtual void OnClientDataReceived(int32_t client_socket, const char* buf, size_t length){}

    void SendToClient(int32_t client_socket, const char* buf, size_t length);

    void SendToAllClients(int32_t client_sender_socket, const char* buf, size_t length);

    void CleanUp();

private:
    std::string ip_address_;
    uint16_t port_;
    uint32_t socket_;

    fd_set main_;

};