// by andrew.la
#pragma once

#include <cstdint>
#include <cfloat>

#include <cassert>

#include <string>
#include <sstream>

#include <type_traits>

#ifdef RELEASE
#undef assert
#define assert(x) 
#endif

/// @todo create STL-like library with my own data structures (It will not be done)
/// @todo сделать заебись

namespace tiny3d {
    // some arithmetic types

    using uint8 = std::uint8_t;
    using int8 = std::int8_t;

    using uint16 = std::uint16_t;
    using int16 = std::int16_t;

    using uint32 = std::uint32_t;
    using int32 = std::int32_t;

    using uint64 = std::uint64_t;
    using int64 = std::int64_t;

    using isize = std::ptrdiff_t;
    using usize = std::size_t;

    using real32 = std::float_t;
    using real64 = std::double_t;

    using std::string;
    using input_sstream = std::istringstream;
    using output_sstream = std::ostringstream;
}
