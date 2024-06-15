#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

#include "array.h"
#include "core.h"
#include "core_definitions.h"
#include "object.h"
#include "primitive.h"
#include "my_string.h"
#include "my_struct.h"

class Struct : public Object {
public:
    Struct(const std::string& name);

    Struct(Struct&& other);

    ~Struct() override = default;

    void AddObject(Object* obj);

    inline size_t GetSize() const { return primitives_.size() + arrays_.size() + strings_.size() + structs_.size(); }
    inline size_t GetPrimitivesCount() const { return primitives_.size(); }
    inline size_t GetArraysCount() const { return arrays_.size(); }
    inline size_t GetStringsCount() const { return strings_.size(); }
    inline size_t GetStructsCount() const { return structs_.size(); }

    void serialize(std::vector<uint8_t>& buffer, size_t& iterator) const override;

private:
    std::vector<Primitive> primitives_;
    std::vector<Array> arrays_;
    std::vector<String> strings_;
    std::vector<Struct> structs_;
};