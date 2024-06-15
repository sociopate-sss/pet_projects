#include "../include/my_struct.h"

Struct::Struct(Struct&& other) : Object(std::move(other)) {
    std::swap(primitives_, other.primitives_);
    std::swap(arrays_, other.arrays_);
    std::swap(strings_, other.strings_);
    std::swap(structs_, other.structs_);
}

Struct::Struct(const std::string& name)
        : Object(name, static_cast<uint8_t>(Core::Wrapper::Struct)) {
            size_ += sizeof(size_t) * 4;
        }

void Struct::AddObject(Object* obj) {
    size_ += obj->GetSize();
    switch (obj->GetWrapper()) {
        case 1:
            primitives_.push_back(std::move(*dynamic_cast<Primitive*>(obj)));
            break;
        case 2:
            arrays_.push_back(std::move(*dynamic_cast<Array*>(obj)));
            break;
        case 3:
            strings_.push_back(std::move(*dynamic_cast<String*>(obj)));
            break;
        case 4:
            structs_.push_back(std::move(*dynamic_cast<Struct*>(obj)));
            break;
    }
}

void Struct::serialize(std::vector<uint8_t>& buffer, size_t& iterator) const{
    Object::serialize(buffer, iterator);

    // Serialises the size of std::vector and its contents
    auto serialize_same_entities = [&]<typename T>(const std::vector<T>& entities) {
        Core::encode<size_t>(buffer, iterator, entities.size());
        for(const auto& item : entities) {
            item.serialize(buffer, iterator);
        }    
    };

    // Primitives serialization
    serialize_same_entities(primitives_);

    // Arrays serialization
    serialize_same_entities(arrays_);

    // Strings serialization
    serialize_same_entities(strings_);

    // Structs serialization
    serialize_same_entities(structs_);

}