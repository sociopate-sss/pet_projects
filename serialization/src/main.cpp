#include <memory>

#include "../include/array.h"
#include "../include/core.h"
#include "../include/my_string.h"
#include "../include/my_struct.h"
#include "../include/primitive.h"

int main() {

    std::unique_ptr<Primitive> i16_test = Primitive::create<int16_t>("integer", Core::Type::I16, 42);
    Core::SerializeNSave(dynamic_cast<Object*>(i16_test.get()));

    std::unique_ptr<Array> array_test = Array::create<uint32_t>("array", Core::Type::u32, std::vector<uint32_t>{1, 2, 3, 4, 5});
    Core::SerializeNSave(dynamic_cast<Object*>(array_test.get()));

    std::unique_ptr<String> string_test = String::create("string", Core::Type::U8, "Hello world!");
    Core::SerializeNSave(dynamic_cast<Object*>(string_test.get()));

    Struct struct_test("my_struct");
    struct_test.AddObject(dynamic_cast<Object*>(i16_test.get()));
    struct_test.AddObject(dynamic_cast<Object*>(array_test.get()));
    struct_test.AddObject(dynamic_cast<Object*>(string_test.get()));
    Core::SerializeNSave(dynamic_cast<Object*>(&struct_test));

    return 0;
}