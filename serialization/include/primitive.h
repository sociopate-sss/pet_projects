#pragma once

#include <cstddef>
#include <memory>
#include <stdint.h>

#include "core.h"
#include "object.h"

class Primitive : public Object {
private: 
    Primitive(const std::string& name, uint8_t wrapper, uint8_t type);
    Primitive(){}

public:
    Primitive(Primitive&& other);
    ~Primitive() override = default;

    template<typename T>
    static std::unique_ptr<Primitive> create(const std::string& name, Core::Type type, T value) {
        std::unique_ptr<Primitive> primitive{
            std::make_unique<Primitive>(Primitive(name,static_cast<uint8_t>(Core::Wrapper::Primitive), 
                                                  static_cast<uint8_t>(type)))
        };
        primitive->data_ = std::make_unique<std::vector<uint8_t>>(std::vector<uint8_t>(sizeof(value)));
        primitive->size_ += sizeof(value);

        size_t iterator = 0;
        Core::encode<T>(*primitive->data_, iterator, value);

        return primitive;
    }

    inline Core::Type GetType() const { return static_cast<Core::Type>(type_); }
    inline std::vector<uint8_t> GetData() const { return *data_; }

    void serialize(std::vector<uint8_t>& buffer, size_t& iterator) const override;

    static Primitive deserialize(const std::vector<uint8_t>& buffer){
        Primitive obj;
        size_t iterator = 0;

        obj.size_ = Core::decode<uint32_t>(buffer, iterator);
        obj.name_length_ = Core::decode<uint32_t>(buffer, iterator);
        obj.name_ = Core::decode<std::string>(buffer, iterator);
        obj.wrapper_ = Core::decode<uint8_t>(buffer, iterator);

        obj.type_ = Core::decode<uint8_t>(buffer, iterator);
        obj.data_ = std::make_unique<std::vector<uint8_t>>(
            std::vector<uint8_t>(GetTypeSize(static_cast<Core::Type>(obj.type_))));
        Core::decode<uint8_t>(buffer, iterator, *obj.data_);

        return obj;
    }

private:
    uint8_t type_;
    std::unique_ptr<std::vector<uint8_t>> data_{nullptr};

};
