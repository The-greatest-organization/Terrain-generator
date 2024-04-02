#pragma once

#include <cstdint>

#ifdef ZeroMemory
#undef ZeroMemory
#endif

#ifdef CopyMemory
#undef CopyMemory
#endif

#ifdef SetMemory
#undef SetMemory
#endif

#define ENUM_VALUE(enumVar) static_cast<Uint64>(enumVar)

#define MAX(a, b) ((a > b)? a : b)
#define MIN(a, b) ((a < b)? a : b)

namespace TerrainGenerator
{
    using Uint8 = uint8_t;
    using Uint16 = uint16_t;
    using Uint32 = uint32_t;
    using Uint64 = uint64_t;

    using Int8 = int8_t;
    using Int16 = int16_t;
    using Int32 = int32_t;
    using Int64 = int64_t;

    using Float32 = float;
    using Float64 = double;

    template <typename t>
    using Ref = t&;

    template <typename t>
    using ConstRef = const t&;

    enum class DataTypes : Uint64
    {
        UnsignedByte,
        SignedByte,
        UnsignedShort,
        SignedShort,
        UnsignedInt,
        SignedInt,

        Float32,
        Float64,
    };
}