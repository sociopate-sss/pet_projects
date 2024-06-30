#include <fstream>
#include <iterator>
#include <vector>

#include "../include/core.h"

void Core::SerializeNSave(Object* obj) {
    std::vector<uint8_t> buffer(obj->GetSize());
    size_t iterator = 0;

    obj->serialize(buffer, iterator);
    std::string file_name = obj->GetName() + ".ttc";
    Save(file_name, buffer);
}

void Core::Save(const std::string& file_name, const std::vector<uint8_t>& buffer) {
    std::ofstream fout(file_name);
    for (size_t i = 0; i < buffer.size(); i++) {
        fout << buffer[i];
    }
    fout.close();
}

std::vector<uint8_t> Core::Load(const std::string& file_name) {
    std::ifstream fin("../build/" + file_name, std::ios::binary);
    std::vector<uint8_t> result((std::istreambuf_iterator<char>(fin)), (std::istreambuf_iterator<char>()));
    return result;
}

uint8_t Core::GetTypeSize(Core::Type type) {
    switch (type) {
        case Core::Type::I8:
        case Core::Type::U8:
            return sizeof(uint8_t);
            break;
        case Core::Type::I16:
        case Core::Type::U16:
            return sizeof(uint16_t);
            break;
        case Core::Type::I32:
        case Core::Type::U32:
            return sizeof(uint32_t);
            break;
        case Core::Type::I64:
        case Core::Type::U64:
            return sizeof(uint64_t);
            break;
        case Core::Type::BOOL:
            return sizeof(bool);
            break;
        case Core::Type::FLOAT:
            return sizeof(float);
            break;
        case Core::Type::DOUBLE:
            return sizeof(double);
            break;
        default:
            return 0;
            break;
    }
}