#include "../include/multiclient_chat.hpp"

int main() {
    MultiClientChat web_server("127.0.0.1", 56000);
    web_server.init();
    web_server.run();
    return 0;
} 