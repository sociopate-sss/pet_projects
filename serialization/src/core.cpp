#include <fstream>
#include <vector>

#include "../include/core.h"

void Core::SerializeNSave(Object* obj) {
    std::vector<uint8_t> buffer(obj->GetSize());
    size_t iterator = 0;

    obj->serialize(buffer, iterator);
    std::string file_name = obj->GetName() + ".ttc";
    Save(file_name, buffer);
}

void Core::Save(std::string file_name, const std::vector<uint8_t>& buffer) {
    std::ofstream fout(file_name);
    for (size_t i = 0; i < buffer.size(); i++) {
        fout << buffer[i];
    }
    fout.close();
}