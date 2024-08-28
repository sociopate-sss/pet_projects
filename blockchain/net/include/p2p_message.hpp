#pragma once

#include "../../utils/json.hpp"

using json = nlohmann::json;

namespace net {

    enum class P2PMessageType : uint8_t {
        ConnectRequest = 0,
        ConfirmConnect,
        TransactionInfo
    };  

    struct P2PMessageHeader{
        P2PMessageType type;
        size_t size = 0;
    };

    struct P2PMessage {
        P2PMessageHeader header;
        json body;
    };

    using MessageHandler = std::unordered_map<P2PMessageType, void(*)(P2PMessage&&)>;
}