#include "../include/base_server.hpp"

int main() {
    net::BaseServer<net::MessageTypes> server(56000);
    //server.Start();
    return 0;
} 