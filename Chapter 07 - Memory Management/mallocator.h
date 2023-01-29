// The infamous "Mallocator" - a stateless allocator
template <class T> struct Mallocator {
    using value_type = T;
    
    Mallocator() = default;
    
    template <class U> Mallocator(const Mallocator<U>&) noexcept;
    
    template <class U> bool operator==(const Mallocator<U>&) const noexcept
    {
        return true;
    }
    
    template <class U> bool operator!=(const Mallocator<U>&) const noexcept
    {
        return false;
    }
    
    T* allocate(std::size_t n) const
    {
        if (!n) { return nullptr; }
        
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
            throw std::bad_array_new_length();
        }
        
        void const *pv = std::malloc(n * sizeof(T));
        
        if (!pv) { throw std::bad_alloc(); }
        
        return static_cast<T*>(pv);
    }
    
    void deallocate(void *p) const noexcept
    {
        std::free(p);
    }
};
