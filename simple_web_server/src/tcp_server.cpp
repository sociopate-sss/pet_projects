#include "../include/tcp_server.hpp"

uint32_t TCPServer::init() {
    WSADATA wsa_data;
    WORD version = MAKEWORD(2, 2);

    int ws_ok = WSAStartup(version, &wsa_data);
    if (ws_ok != 0) {
        return ws_ok;
    }

    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ == SOCKET_ERROR) {
        return WSAGetLastError();
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(56000);
    inet_pton(AF_INET, ip_address_.c_str(), &addr.sin_addr);

    if (bind(socket_, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
        return WSAGetLastError();
    }

    if (listen(socket_, SOMAXCONN) == SOCKET_ERROR) {
        return WSAGetLastError();
    }

    FD_ZERO(&main_);

    FD_SET(socket_, &main_);

    return 0;
}

uint32_t TCPServer::run() {

    char buf[4096];

    bool is_active = true;

    while(is_active) {
        fd_set copy_main = main_;

        int socket_count = select(0, &copy_main, nullptr, nullptr, nullptr);

        for (size_t i = 0; i < socket_count; ++i) {

            SOCKET cur_sock = copy_main.fd_array[i];

            if (cur_sock == socket_) {
                SOCKET new_client_sock = accept(socket_, nullptr, nullptr);
                FD_SET(new_client_sock, &main_);

                OnClientConnected(new_client_sock);
                
            } else {
                memset(buf, 0, 4096);

                int bytes_received = recv(cur_sock, buf, 4096, 0);

                if (bytes_received <= 0) {

                    OnClientDisconnected(cur_sock);
                    closesocket(cur_sock);
                    FD_CLR(cur_sock, &main_);

                } else {
                    OnClientDataReceived(cur_sock, buf, bytes_received);
                }

            }
        }

    }

    CleanUp();
    
    return 0;
}

void TCPServer::CleanUp() {
    FD_CLR(socket_, &main_);
    closesocket(socket_);

    for (int i = main_.fd_count - 1; i >= 0; --i) {
        FD_CLR(main_.fd_array[i], &main_);
        closesocket(main_.fd_array[i]);
    }

    WSACleanup();
}

void TCPServer::SendToClient(int32_t client_socket, const char* buf, size_t length) {
    send(client_socket, buf, length, 0);
}

void TCPServer::SendToAllClients(int32_t client_sender_socket, const char* buf, size_t length) {
    for (size_t i = 0; i < main_.fd_count; ++i) {
        SOCKET cur_sock = main_.fd_array[i];

        if(cur_sock != socket_ && cur_sock != client_sender_socket) {
            SendToClient(cur_sock, buf, length);
        }
    }
}