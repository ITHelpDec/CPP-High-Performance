#include <array>
#include <iostream>
#include <mutex>

template <typename T, int N>
class BoundedBuffer {

private:
    std::mutex mtx_;

    std::array<T, N> buffer_;

    std::size_t read_pos_, write_pos_;

    void do_pushT&& item)
    {
        std::unique_lock lock(mtx_);
        buffer_[write_pos_] = 
    }
};