#include <experimental/coroutine>
#include <iostream>

template <typename T>
class Generator {
    struct Promise;
    struct Sentinel { };
    struct Iterator;
public:
    using promise_type = Promise;
    
    Generator(Generator &&g) { std::exchange(g.h_, h_); }
    
    ~Generator()
    {
        if (h_) { h_.destroy(); }
    }
    
    auto begin()
    {
        h_.resume();
        return Iterator{h_};
    }
    
    auto end() { return Sentinel(); }
private:
    std::experimental::coroutine_handle<Promise> h_;
    explicit Generator(std::experimental::coroutine_handle<Promise> h) : h_{h} { }
};

template <typename T>
struct Generator<T>::Promise {
    T value_;
    
    Generator get_return_object()
    {
        using Handle = std::experimental::coroutine_handle<Promise>;
        return Generator{Handle::from_promise(*this)};
    }
    
    auto initial_suspend() { return std::experimental::suspend_always{}; }
    auto final_suspend() noexcept { return std::experimental::suspend_always{}; }
    
    void return_void() {}
    void unhandled_exception() { throw; }
    
    // auto yield_value(T&&);
    // auto yield_value(const T&);
    
    auto yield_value(auto &&value)
    {   // why not use technique from before?
        value_ = std::forward<decltype(value)>(value);
        return std::experimental::suspend_always();
    }
};

template <typename T>
struct Generator<T>::Iterator {
    using iterator_category = std::input_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    
    std::experimental::coroutine_handle<Promise> h_; // Data member
    
    Iterator& operator++()
    {
        h_.resume();
        return *this;
    }
    
    void operator++(int) { (void)operator++(); }
    
    T operator*() const { return h_.promise().value_; }
    T* operator->() const { return std::addressof(operator*()); }
    
    bool operator==(Sentinel) const { return h_.done(); }
};

template <typename T> Generator<T> seq() {
    for (T i = { }; /*...*/ ; ++i) {
        co_yield i;
    }
}
template <typename T>
Generator<T> take_until(Generator<T> &gen, T value) {
    for (auto &&v : gen) {
        if (v == value) { co_return; }
        co_yield v;
    }
}

template <typename T>
Generator<T> add(Generator<T> &gen, T increment) {
    for (auto &&v : gen) { co_yield v + increment; }
}

int main()
{
    auto s = seq<int>();
    auto t = take_until<int>(s, 10);
    auto a = add<int>(t, 3);
    
    int sum = 0;
    
    // delta increases by 1 every time (+3, +4, +5, etc...)
    for (auto &&v : a) { sum += v; std::cout << sum << ' '; }
    
    return sum;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// 3 7 12 18 25 33 42 52 63 75
// Program ended with exit code: 75
