template <typename T>
auto lin_value(T start, T stop, std::size_t index, std::size_t n) {
    assert(n > 1 && index < n);
    const auto amount = static_cast<T>(index) / (n - 1);
    const auto v = std::lerp(start, stop, amount); // C++20 - code will not run without this line...
    return v;
}

template <typename T>
struct LinSpace {
    LinSpace(T start, T stop, std::size_t n) : begin_{start, stop, 0, n}, end_{n} {}
    struct Iterator {
        using difference_type = void;
        using value_type = T;
        using reference = T;
        using pointer = T*;
        using iterator_category = std::forward_iterator_tag;
        void operator++() { ++i_; }
        T operator*() { return lin_value(start_, stop_, i_, n_);}
        bool operator==(std::size_t i) const { return i_ == i; }
        T start_{};
        T stop_{};
        std::size_t i_{};
        std::size_t n_{};
    };
    auto begin() { return begin_; }
    auto end() { return end_; }
private:
    Iterator begin_{};
    std::size_t end_{};
};

template <typename T>
auto lin_space(T start, T stop, size_t n) {
    return LinSpace{start, stop, n};
}

int main()
{
    for (auto v : lin_space(2.0f, 3.0f, 5)) {
        std::cout << v << ' ';
    } std::cout << '\n';
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// 2 2.25 2.5 2.75 3
// Program ended with exit code: 0
