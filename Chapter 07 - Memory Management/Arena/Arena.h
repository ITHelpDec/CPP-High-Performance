#include <iostream>

template <std::size_t N>
class Arena {
public:
    Arena() noexcept : ptr_(buffer_) { }
    
    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;
    
    void reset() noexcept { ptr_ = buffer_; }
    
    static constexpr std::size_t size() noexcept { return N; }
    
    std::size_t used() const noexcept
    {
        return static_cast<std::size_t>(ptr_ - buffer_);
    }
    
    std::byte* allocate(std::size_t);
    void deallocate(std::byte*, std::size_t) noexcept;
    
private:
    static constexpr std::size_t alignment = alignof(std::max_align_t);
    
    // size guaranteed at compile-time
    alignas(alignment) std::byte buffer_[N];
    std::byte *ptr_;
    
    static std::size_t align_up(std::size_t n) noexcept
    {
        return (n + (alignment - 1)) & ~(alignment - 1);
    }
    
    // compares a pointer address with the address range of the arena
    bool pointer_in_buffer(const std::byte *p) const noexcept
    {
        // cast to std::uintptr_t before comparison to avoid undefined behaviour
        return std::uintptr_t(p) >= std::uintptr_t(buffer_) &&
               std::uintptr_t(p) < (std::uintptr_t(buffer_) + N);
    }
};

// returns pointer to correctly-aligned memory
template <std::size_t N> std::byte* Arena<N>::allocate(std::size_t n)
{
    const std::size_t aligned_n = align_up(n);
    const std::size_t available_bytes =
        static_cast<decltype(aligned_n)>(buffer_ + N - ptr_);
    
    if (available_bytes >= aligned_n) {
        std::byte *r = ptr_;
        ptr_ += aligned_n;
        return r;
    }
    
    // fall back to ::opeartor new if no space in buffer
    return static_cast<std::byte*>(::operator new(n));
}

template <std::size_t N>
void Arena<N>::deallocate(std::byte *p, std::size_t n) noexcept
{   // on the buffer?
    if (pointer_in_buffer(p)) {
        n = align_up(n);
        if (p + n == ptr_) { ptr_ = p; }
    } else {
        ::operator delete(p);
    }
}
