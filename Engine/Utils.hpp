#pragma once

#include <cstdint>
#include <cfloat>
#include <memory>

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
    using real128 = __float128;

    // memory types
    /// @todo add custom allocators to my engine (STL compatible)

    using std::make_unique;
    using std::make_shared;

    using std::shared_ptr;
    using std::unique_ptr;

    template <typename T>
    using raw_ptr = T*;

    // string types
    /// @todo add custom string classes (and custom STL with custom allocators)

    using std::string;
    using input_sstream = std::istringstream;
    using output_sstream = std::ostringstream;

    // exception classes & functions
    /// @todo create class with exceptions and with posibility to turn it off

    using arg_error = std::invalid_argument;
    using index_error = std::out_of_range;
    using std::logic_error;
    using std::runtime_error;

    template <typename E>
    void throw_if(bool condition, const E& exception) {
        static_assert(std::is_base_of_v<std::exception, E>);
        static_assert(std::is_copy_constructible_v<E>);

        if (!condition)
            throw E {exception};
    }
}
