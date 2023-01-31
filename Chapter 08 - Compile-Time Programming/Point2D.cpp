#include <concepts>
#include <cmath>
#include <iostream>

template <typename T, typename U>
class Point2D {
public:
    Point2D(T x, U y) : x_(x), y_(y) { }
    
    auto x() const { return x_; }
    auto y() const { return y_; }
    
private:
    T x_;
    U y_;
};

template <typename T, typename U, typename V, typename W>
auto dist(const Point2D<T, U> &p1, const Point2D<V, W> &p2) {
    auto a = abs(p2.y() - p1.y());
    auto b = abs(p2.x() - p1.x());
    return std::sqrt( (a * a) + (b * b) );
}

auto auto_dist(const auto &p1, const auto &p2) {
    auto a = abs(p2.y() - p1.y());
    auto b = abs(p2.x() - p1.x());
    return std::sqrt( (a * a) + (b * b) );
}

int main()
{
    Point2D p1 = { 2.0, 2 };
    Point2D p2 = { 6, 5.0 };
    
    std::cout << dist(p1, p2) << '\n';
    std::cout << auto_dist(p1, p2) << '\n';
    
    // std::cout << dist("woof", "miaow") << '\n';
    // std::cout << auto_dist("woof", "miaow") << '\n';
    
    return 0;
}
