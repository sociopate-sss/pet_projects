#include "../include/net_utils.hpp"
#include "asio/ip/address.hpp"
#include <string>

// void net::HTTPHeaderToJson(std::istream& buffer, json& http_header){

//     std::string str;
//     bool is_first = true;

//     do {
//         if (str.size() && str.back() == '\r') {
//             str.pop_back();
//         }

//         size_t start_ptr = 0;
//         size_t end_ptr = 0;

//         auto get_next_until = [&](const std::string& delim) {

//             end_ptr = str.find(delim, start_ptr);
//             std::string result = str.substr(start_ptr, end_ptr - start_ptr);
//             start_ptr = end_ptr + delim.size();

//             return result;
//         };

//         if (is_first) {

//             if (str.back() != ' ') {
//                 str += " ";
//             }

//             for (const std::string& key : {"method", "url", "version"}) {
//                 http_header[key] = get_next_until(" ");
//             }

//             is_first = false;

//             continue;   
//         }

//         if (str.back() != '\n') {
//             str += "\n";
//         }

//         http_header[get_next_until(": ")] = get_next_until("\n");

//     } while (std::getline(buffer, str, '\n'));
// }

std::vector<asio::ip::tcp::endpoint> net::EndpointsToVector(const std::string &str) {
    std::vector<asio::ip::tcp::endpoint> endpoints;
    size_t start_ptr = 0;
    size_t end_ptr = 0;

    auto get_next_until = [&](const std::string& delim) {

        end_ptr = str.find(delim, start_ptr);
        if(end_ptr == std::string::npos) {
            end_ptr = str.size();
        }

        std::string result = str.substr(start_ptr, end_ptr - start_ptr);
        start_ptr = end_ptr + delim.size();

        return result;
    };

    while(std::max(end_ptr, start_ptr) < str.size()) {
        std::string endpoint_str = get_next_until(" ");
        size_t delim = endpoint_str.find(":");

        if (delim == std::string::npos) {
            continue;
        }

        std::string ip_address_str = endpoint_str.substr(0, delim);

        try {
            auto ip_address = asio::ip::make_address(ip_address_str);
            uint16_t port = std::stoi(endpoint_str.substr(delim + 1, endpoint_str.size() - (delim + 1)));
            asio::ip::tcp::endpoint endpoint(ip_address, port);
            endpoints.push_back(endpoint);
        } catch(const asio::error_code& ec) {
            continue;
        } catch (const std::invalid_argument& e) {
            continue;
        } catch (const std::out_of_range& e) {
            continue;
        }
    }

    return endpoints;
}