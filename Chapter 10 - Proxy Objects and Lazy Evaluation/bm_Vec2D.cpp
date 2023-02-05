#include <cmath>
#include <iostream>
#include <vector>
#include <random>

class Vec2D {
    friend float min_length();
public:
    Vec2D(float x, float y) : x_(x), y_(y) { }
    
    float length_squared() const
    {
        return (x_ * x_) + (y_ * y_);
    }
    
    float length() const
    {
        return std::sqrt(length_squared());
    }
    
private:
    float x_, y_;
};

float smallestVec2D_min_e(const std::vector<Vec2D> &vec) {
    assert(!vec.empty());
    return std::min_element(vec.begin(), vec.end(), [] (const Vec2D &lhs, const Vec2D &rhs)
                            { return lhs.length_squared() < rhs.length_squared(); } )->length();
}

float smallestVec2D(const std::vector<Vec2D> &vvec) {
    assert(!vvec.empty());
    Vec2D smallest = vvec.front();
    std::for_each(vvec.begin(), vvec.end(), [&] (const Vec2D &other) {
        smallest = (std::fmin(smallest.length_squared(), other.length_squared()) == smallest.length_squared() ? smallest : other); } );
    return smallest.length();
}

// auto min_length(const auto& r) -> float {
//   assert(!r.empty());
//   auto cmp = [](auto&& a, auto&& b) {
//     return a.length () < b.length();
//   };
//   auto it = std::ranges::min_element(r, cmp);
//   return it->length();
// }

int coord() {
    static std::default_random_engine e;
    static std::uniform_int_distribution u;
    return u(e);
}

std::vector<Vec2D> gen(std::size_t n) {
    std::vector<Vec2D> vvec;
    vvec.reserve(n);
    
    for (std::size_t i = 0; i != n; ++i) {
        vvec.push_back(Vec2D(coord(), coord()));
    }
    
    return vvec;
}

int main()
{
    std::cout << smallestVec2D(gen(10000)) << '\n';       // 4.24264
    std::cout << smallestVec2D_min_e(gen(10000)) << '\n'; // 4.24264
    
    return 0;
}
