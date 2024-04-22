#pragma once

#include "Utils.hpp"
#include <memory>

namespace tiny3d {
    using std::unique_ptr;
    using std::shared_ptr;
    template <typename T> using raw_ptr = T*;

    struct Allocator {
        Allocator() = delete;

        Allocator(usize size);

        Allocator(const Allocator& allocator);
        Allocator(Allocator&& allocator) noexcept;

        Allocator& operator=(const Allocator& allocator);
        Allocator& operator=(Allocator&& allocator) noexcept;

        /// @todo add 'maybe_unused' attribute to alignment parameter

        virtual void* Allocate(usize size, usize alignment) = 0;
        virtual void* Reallocate(void* ptr, usize new_size) = 0;
        virtual void Free(void* ptr) = 0;

        virtual ~Allocator();

        protected:
        using Byte = uint8;
        using BytePtr = uint8*;

        usize arena_size_;
        BytePtr arena_addr_;

        usize total_allocations_;
        usize total_allocated_;
    };

    struct LinearAllocator : Allocator {
        using Allocator::Allocator;

        void* Allocate(usize size, usize alignment);
        void* Reallocate(void* ptr, usize new_size);
        void Free(void* ptr);

        private:
        BytePtr next_addr_;
        BytePtr last_addr_;
    };

    struct StackAllocator : Allocator {
        using Allocator::Allocator;

        void* Allocate(usize size, usize alignment);
        void* Reallocate(void* ptr, usize new_size);
        void Free(void* ptr);

        private:
        BytePtr next_addr_;
        BytePtr end_addr_;
    };

    /// @todo Realize a BigBoy Allocator with so many parameters and optimizations

    struct PoolAllocator : Allocator {
        struct Desc {};

        PoolAllocator() = delete;

        PoolAllocator(const Desc& desc);

        PoolAllocator(const PoolAllocator& allocator);
        PoolAllocator(PoolAllocator&& allocator) noexcept;

        void* Allocate(usize size, usize alignment);
        void* Reallocate(void* ptr, usize new_size);
        void Free(void* ptr);
    };

    /// @todo Realize STL adaptor for allocators
}
