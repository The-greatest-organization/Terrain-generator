#include "Memory.hpp"

namespace tiny3d {
    Allocator::Allocator(usize size) {
        assert(size > 0);

        arena_size_ = size;
        arena_addr_ = new Byte[size];

        total_allocated_ = 0;
        total_allocations_ = 0;
    }

    Allocator::Allocator(Allocator&& allocator) noexcept {
        assert(allocator.arena_addr_ != nullptr);
        assert(allocator.arena_size_ > 0);

        arena_addr_ = allocator.arena_addr_;
        arena_size_ = allocator.arena_size_;

        total_allocated_ = 0;
        total_allocations_ = 0;

        allocator.arena_addr_ = nullptr;
        allocator.arena_size_ = 0;
    }

    Allocator::~Allocator() {
        delete[] arena_addr_;
    }
}
