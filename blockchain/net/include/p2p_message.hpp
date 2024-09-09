#pragma once

#include "../../utils/include/json.hpp"
#include "../../utils/include/thread_safe_queue.hpp"

#include <memory>

using json = nlohmann::json;

namespace net {

    class Connection;

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

        /*
            body fields:
                
                # In all messages
                - type          : P2PMessageType
                - id            : uint64_t  
                - sender        : asio::ip::tcp::endpoint
                - for_all       : bool

                # Extra fields in messages of Transaction type
                - info   : json
        */
        json body;

        P2PMessageType GetType() const { return header.type; }
        size_t GetSize() const { return header.size; }
    };

    using ConnPtr = std::shared_ptr<Connection>;

    struct LinkedP2PMessage : public P2PMessage{
        ConnPtr connection_;
    };

    using ConnectionPtrs = std::vector<std::shared_ptr<Connection>>;

    using LinkedMessageQueue = std::shared_ptr<ThreadSafeQueue<LinkedP2PMessage>>;
    using MessageQueue = std::unique_ptr<ThreadSafeQueue<P2PMessage>>;
}