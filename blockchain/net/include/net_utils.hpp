#pragma once

#define ASIO_STANDALONE

#ifdef _WIN32
    #define _WIN32_WINNT 0x0A00
#endif

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include "../../utils/json.hpp"

using json = nlohmann::json;

namespace net {

    void HTTPHeaderToJson(std::istream& str, json& http_header);

    std::vector<asio::ip::tcp::endpoint> EndpointsToVector(const std::string& str);

}