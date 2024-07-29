#pragma once 

#include <stdint.h>
#include <string>
#include <vector>

class Block {
private:
    uint64_t index_;
    uint8_t difficulty_level_;

    std::string prev_hash_;
    std::string hash_;
    std::string nonce_;

    std::vector<std::string> transactions_;
    // std::vector for merkle tree
public:
    Block(uint64_t index, const std::string& prev_hash, const std::string& hash, const std::string& nonce, uint8_t difficulty_level);
    
    void AddTransaction(std::string transaction);

    inline std::string GetHash() const { return hash_; }
    inline std::string GetPrevHash() const { return prev_hash_; }
};