#include <cassert>

struct Number {
    Number(int &&val) : val_(std::move(val)) { }
    int val_;
};

bool operator==(const Number &lhs, const Number &rhs) { return lhs.val_ == rhs.val_; }
bool operator!=(const Number &lhs, const Number &rhs) { return !(lhs == rhs); }

class Widget {
public:
    Widget(const Number &x, const Number &y) : x_(x), y_(y) { assert(are_valid()); }
    
    void update(const Number&, const Number&);
    void cop_n_swap_update(const Number&, const Number&);
private:
    Number x_, y_;
    
    bool are_valid() const { return x_ != y_; }
};

void Widget::update(const Number &x, const Number &y)
{
    assert(x != y && are_valid());
    x_ = x;
    y_ = y;
    assert(are_valid());
}

// this feels like a waste of resources for the same outcome
void Widget::cop_n_swap_update(const Number &x, const Number &y)
{
    assert(x != y && are_valid());
    // auto x_tmp = x, y_tmp = y;
    Number x_tmp = x, y_tmp = y;
    std::swap(x_tmp, x_);
    std::swap(y_tmp, y_);
    assert(are_valid());
}

int main() {
    Number x(6), y(9);
    Widget widget(x, y);
    
    x = 8;
    widget.cop_n_swap_update(x, y);
    
    x = 7;
    widget.update(x, y);
    
    y = 7;
    widget.update(x, y);
}
