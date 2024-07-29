#include "../include/transaction.hpp"

Transaction::Transaction(const std::string& inTX, const std::string& outTX, uint64_t amount)
            :
            inTX_(inTX), outTX_(outTX), amount_(amount) {}

