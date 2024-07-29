#include "../include/block.hpp"

Block::Block(uint64_t index, const std::string& prev_hash, const std::string& hash, const std::string& nonce, uint8_t difficulty_level)
        : 
        index_(index), prev_hash_(prev_hash), hash_(hash),
        nonce_(nonce), difficulty_level_(difficulty_level) {}
