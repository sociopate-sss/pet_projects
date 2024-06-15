#pragma once

namespace Core {

    // Core definitions

    enum class Wrapper{
        Object = 0,
        Primitive,
        Array,
        String,
        Struct  
    };

    enum class Type {
        I8 = 1,
        I16,
        I32,
        I64,

        U8,
        U16,
        u32,
        U64,

        BOOL,

        FLOAT,
        DOUBLE,
    };
}