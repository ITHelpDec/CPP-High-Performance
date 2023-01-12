# C++ High Performance (2nd Edition)

## Highlights from Chapter 2 - "Essential C++ Techniques"

### Using auto in function signatures
Controversial opinion here, but whilst helpful in certain instances, I'm not a fan of the (seemingly-)recent fetishisation of `auto` in code.

By all means, if generics are important, and the type can be interpreted easily (e.g. iterators), then that's a good thing for readiblity, but it most instances where programmers use it like `var` in JavaScript, I find it unecessarily vague and ambiguous when it comes to debugging.

From what I've read, `auto` doesn't seem to hamper performance (which is great), but I really like the idea of code being self-documenting, and this, for me, doesn't tick that box.

Two examples that came to mind were from page 45 and page 65.
```cpp
auto load_record(std::uint32_t id) {
    assert(id);
    auto record = read(id);
    assert(record.is_valid());
    return record;
} // what is "record"?
```
```cpp
auto v = 3; // int
auto lambda = [v](auto v0, auto v1) {
  return v + v0*v1;
}; // why comment "int"? why not write it? do we want int? what if we wanted std::size_t or a short?
```
Given that deterministic destruction was praised by developers for being predictable, can we really say the same for `auto`?

`decltype(auto)` is useful for reducing repitition in function return types, although I would usually see this is a trailing return type capacity.
#
### Const propagation for pointers
We can use `std::experimental::propogate_const` to _"generate compilation errors when trying mutate an object inside a const function"_ – pg. 23
