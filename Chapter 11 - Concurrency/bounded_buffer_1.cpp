#include <array>
#include <iostream>
#include <mutex>
#include <thread>
#include <optional>

// incomplete - see _2
template <typename T, int N>
class BoundedBuffer {
public:
    void push(const T &t) { do_push(t); }
    void push(T &&t) { do_push(std::move(t)); }
    
    auto pop()
    {
        std::optional<T> item;
        
        {
            std::unique_lock lock(mtx_);
            item = std::move( buffer_[read_pos_] );
            read_pos_ = (read_pos_  + 1) % N;
        }
        
        return std::move(*item);
    }
    
    std::size_t size() { return buffer_.size(); }
private:
    std::array<T, N> buffer_;
    std::size_t read_pos_ = 0, write_pos_ = 0;
    std::mutex mtx_;
    
    // clever helper function
    void do_push(auto &&item)
    {
        std::unique_lock lock(mtx_);
        buffer_[write_pos_] = std::forward<decltype(item)>(item);
        write_pos_ = (write_pos_ + 1) % N;
    }
};

int main()
{
    BoundedBuffer<int, 256> bb;
    
    for (int i = 0; i != bb.size(); ++i) {
        std::thread t1( [&, i] () { bb.push(i); } );
        std::thread t2( [&, i] () { bb.pop();   } );
        
        t1.join();
        t2.join();
    }
    
    return 0;
}
