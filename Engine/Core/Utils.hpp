#pragma once

#include <cstdint>
#include <cfloat>

#include <cassert>

#include <string>
#include <sstream>

#include <type_traits>

/// @todo create different headers for each system

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

    /// @todo replace this freaky defines to normal data types (if such types exist)

    using real32 = _Float32;
    using real64 = _Float64;

    // string types
    /// @todo add custom string classes (and custom STL with custom allocators)

    using std::string;
    using input_sstream = std::istringstream;
    using output_sstream = std::ostringstream;
}
