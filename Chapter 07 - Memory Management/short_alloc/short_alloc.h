// a simplified version of Howard Hinnant's `short_alloc`

#ifndef short_alloc_h
#define short_alloc_h

#include "../Arena/Arena.h"

template <class T, std::size_t N> class short_alloc {
    template <class U, std::size_t M> friend class short_alloc;
public:
    using value_type = T;
    using arena_type = Arena<N>;
    
    short_alloc(const short_alloc&) = default;
    short_alloc& operator=(const short_alloc&) = default;
    
    short_alloc(arena_type& arena) noexcept : arena_(&arena) { }
    
    template <class U>
    short_alloc(const short_alloc<U, N> &other) noexcept : arena_(other.arena_) { }
    
    template <class U>
    struct rebind {
        using other = short_alloc<U, N>;
    };
    
    T* allocate(std::size_t n)
    {
        return reinterpret_cast<T*>(arena_->allocate(n * sizeof(T)));
    }
    
    void deallocate(T* p, std::size_t n) noexcept
    {
        arena_->deallocate(reinterpret_cast<std::byte*>(p), n * sizeof(T));
    }
    
    template <class U, std::size_t M>
    bool operator==(const short_alloc<U, M> &other) const noexcept
    {
        return N == M && arena_ == other.arena_;
    }
    
    template <class U, std::size_t M>
    bool operator!=(const short_alloc<U, M> &other) const noexcept
    {
        return !(*this == other);
    }
private:
    arena_type* arena_;
};

#endif /* short_alloc_h */
