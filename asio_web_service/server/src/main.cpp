#include "../include/my_server.hpp"

int main() {
    net::MyServer server(56000);
    server.Start();

    bool is_quit = false;

    while (!is_quit) {
        server.Update();
    }
    return 0;
} 