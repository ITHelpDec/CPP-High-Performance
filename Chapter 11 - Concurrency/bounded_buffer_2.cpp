#include <array>
#include <iostream>
#include <mutex>
#include <thread>
#include <optional>
#include <semaphore> // new

template <typename T, int N>
class BoundedBuffer {
public:
    void push(const T &t) { do_push(t); }
    void push(T &&t) { do_push(std::move(t)); }
    
    auto pop()
    {
        n_full_slots_.acquire(); // new
        
        std::optional<T> item;
        
        try {
            std::unique_lock lock(mtx_);
            item = std::move( buffer_[read_pos_] );
            read_pos_ = (read_pos_  + 1) % N;
            std::cout << "read_pos:  " << read_pos_ << ", N: " << N << '\n';
        } catch (...) {
            n_full_slots_.release(); // new
            throw;
        } n_empty_slots_.release(); // new
        
        // {
        //     std::scoped_lock<std::mutex> lock(mtx_);
        //     write_pos_ = read_pos_;
        // }
        
        return std::move(*item);
    }
    
    std::size_t size() { return buffer_.size(); }
    
    T& operator[](std::size_t i) { return buffer_[i]; }
    
private:
    std::array<T, N> buffer_;
    std::size_t read_pos_ = 0, write_pos_ = 0;
    std::mutex mtx_;
    
    std::counting_semaphore<N> n_empty_slots_{N}, n_full_slots_{0}; // new
    
    void do_push(auto &&item)
    {
        n_empty_slots_.acquire(); // new
        try { // new
            std::unique_lock lock(mtx_);
            buffer_[write_pos_] = std::forward<decltype(item)>(item);
            write_pos_ = (write_pos_ + 1) % N;
            std::cout << "write_pos: " << write_pos_ << ", N: " << N << '\n';
        } catch (...) {
            n_empty_slots_.release(); // new
            throw;
        }
        
        // {
        //     std::scoped_lock<std::mutex> lock(mtx_);
        //     read_pos_ = write_pos_;
        // }
        
        n_full_slots_.release();
    }
};

int main()
{
    BoundedBuffer<int, 8> bb;
    
    for (int i = 0; i != 8; ++i) {
        std::thread t1( [&, i] () { bb.push(i); } );
        std::thread t2( [&, i] () { bb.pop(); } );
        
        t1.join();
        t2.join();
    }
    
    std::cout << "buffer: ";
    for (int i = 0; i != 8; ++i) { std::cout << bb[i] << ' '; } std::cout << '\n';
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// write_pos: 1, N: 8
// read_pos:  1, N: 8
// write_pos: 2, N: 8
// read_pos:  2, N: 8
// write_pos: 3, N: 8
// read_pos:  3, N: 8
// write_pos: 4, N: 8
// read_pos:  4, N: 8
// write_pos: 5, N: 8
// read_pos:  5, N: 8
// write_pos: 6, N: 8
// read_pos:  6, N: 8
// write_pos: 7, N: 8
// read_pos:  7, N: 8
// write_pos: 0, N: 8
// read_pos:  0, N: 8
// buffer: 0 1 2 3 4 5 6 7
// Program ended with exit code: 0
