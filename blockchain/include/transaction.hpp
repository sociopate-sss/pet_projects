#pragma once

#include <string>
#include <cstdint>

class Transaction {
private:
    std::string inTX_;
    std::string outTX_;
   
    uint64_t amount_;

public:
    Transaction(const std::string& inTX, const std::string& outTX, uint64_t amount);

    inline std::string ToString() const { return "inTX: " + inTX_ + ", outTX: " + outTX_ + ", amount: " + std::to_string(amount_); }
};