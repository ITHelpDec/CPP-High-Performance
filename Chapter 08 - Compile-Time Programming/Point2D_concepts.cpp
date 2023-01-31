#include <concepts>
#include <iostream>
#include <cmath>

template <typename T, typename U>
class Point2D {
public:
    Point2D(T x, U y) : x_(x), y_(y) { }
    
    T x() const { return x_; }
    U y() const { return y_; }
private:
    T x_;
    U y_;
};

template <typename T>
concept Point = requires(T p) {
    // don't need to be the same type, just need to be arithmetic
    // this is actually quite useful -
    requires std::is_arithmetic_v<decltype(p.x())> &&
             std::is_arithmetic_v<decltype(p.y())>;
};

auto dist(Point auto p1, Point auto p2) {
    auto a = abs(p2.y() - p1.y());
    auto b = abs(p2.x() - p1.x());
    return std::sqrt( (a * a) + (b * b) );
}

int main()
{
    Point2D p1 = { 2, 2 };
    Point2D p2 = { 6, 5 };
    
    std::cout << dist(p1, p2) << '\n';
    std::cout << dist(p2, p1) << '\n';
    
    Point2D p3 = { 2, 2.2 };
    Point2D p4 = { 6.3, 5 };
    
    // now works with different types, like my previous template design
    std::cout << dist(p3, p4) << '\n';
    
    // fails when supplying strings
    // std::cout << dist("woof", "miaow") << '\n';
    
    return 0;
}
