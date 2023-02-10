#include <iostream>
#include <array>
#include <atomic>
#include <optional>

template <typename T, std::size_t N>
class LockFreeQueue {
public:
    // "writer" thread functions
    bool push(const T &t) { do_push(t); }
    bool push(T &&t) { do_push(std::move(t)); }
    
    // "reader" thread functions
    std::optional<T> pop()
    {
        std::optional<T> val;
        
        if (size_.load() > 0) {
            val = std::move( buffer_[read_pos_] );
            read_pos_ = (read_pos_ + 1) % N;
            --size_;
        }
        
        return val;
    }
    
    // both threads can call size()
    std::size_t& size() const noexcept { return size_.load(); }
    
private:
    std::array<T, N> buffer_;               // used by both threads
    std::atomic<std::size_t> size_ = 0;     // used by both threads
    
    std::size_t read_pos_  = 0;             // used by "reader" thread
    std::size_t write_pos_ = 0;             // used by "writer" threads
    
    static_assert(std::atomic<std::size_t>::is_always_lock_free);
    
    bool do_push(auto &&t) {
        if (size_.load() == N) { return false; }
        
        buffer_[write_pos_] = std::forward<decltype(t)>(t);
        write_pos_ = (write_pos_ + 1) % N;
        
        ++size_;
        
        return true;
    }
};
