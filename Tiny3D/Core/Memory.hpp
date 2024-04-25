// by andrew.la
#pragma once

#include "Utils.hpp"
#include <memory>

namespace tiny3d {
    template <typename T> using unique_pointer = std::unique_ptr<T>;
    template <typename T> using shared_pointer = std::shared_ptr<T>;
    template <typename T> using weak_pointer = std::weak_ptr<T>;
    template <typename T> using raw_ptr = T*;

    struct Allocator {
        Allocator() = delete;

        Allocator(usize size);

        Allocator(const Allocator& allocator) = delete;
        Allocator(Allocator&& allocator) noexcept;

        Allocator& operator=(const Allocator& allocator) = delete;
        Allocator& operator=(Allocator&& allocator) = delete;

        /// @todo add 'maybe_unused' attribute to alignment parameter

        virtual void* Allocate(usize size, usize alignment) = 0;
        virtual void* Reallocate(void* ptr, usize new_size) = 0;
        virtual void Free(void* ptr) = 0;

        virtual ~Allocator();

        protected:
        using Byte = uint8;
        using BytePtr = raw_ptr<uint8>;

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

    /// @todo Realize a pool allocator with so many parameters and optimizations
    // Big boy allocator

    struct PoolAllocator : Allocator {

        // Pool parameters
        struct Desc {
        };

        PoolAllocator() = delete;

        PoolAllocator(const Desc& desc);

        PoolAllocator(const PoolAllocator& allocator) = delete;
        PoolAllocator(PoolAllocator&& allocator) = delete;

        void* Allocate(usize size, usize alignment);
        void* Reallocate(void* ptr, usize new_size);
        void Free(void* ptr);
    };
}
