#include <string>
#include <iostream>

struct ConcatProxy {
    // creating a constructor does not impact the size of the struct
    // ConcatProxy(const auto &a, const auto &b) : a_(a), b_(b) { }
    const std::string &a_;
    const std::string &b_;
};

struct String {
    String() = default;
    
    // change from std::string to const char* to allow it to work with main()
    String(const char *str) : str_(std::move(str)) { }
    
    std::string str_;
};

ConcatProxy operator+(const String &a, const String &b)
{
    // cannot use normal parentheses here without struct constructors
    // return ConcatProxy(a.str_, b.str_);
    return ConcatProxy{a.str_, b.str_};
}

bool compare_proxy(const std::string &a, const std::string &b, const std::string &c)
{
    return a.size() + b.size() == c.size() &&
           std::equal(a.begin(), a.end(), c.begin()) &&
           std::equal(b.begin(), b.end(), c.begin() + a.size());
}

bool operator==(ConcatProxy &&proxy, const String &rhs)
{
    return compare_proxy(proxy.a_, proxy.b_, rhs.str_);
}

int main()
{
    std::cout << std::boolalpha;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    std::string a1 = "Cole", b1 = "Porter", c1 = "ColePorter";
    std::cout << "(a1 + b1) == c1: " << ( (a1 + b1) == c1 ) << '\n';
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    String a2 = "Cole", b2 = "Porter", c2 = "ColePorter";
    std::cout << "(a2 + b2) == c2: " << ( (a2 + b2) == c2 ) << '\n';
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    std::cout << std::noboolalpha;
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// (a1 + b1) == c1: true
// (a2 + b2) == c2: true
// Program ended with exit code: 0
