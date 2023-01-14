# C++ High Performance (2nd Edition)

## Highlights from Chapter 2 - "Essential C++ Techniques"

### Using auto in function signatures
Controversial opinion here, but whilst helpful in certain instances, I'm not a fan of the (seemingly-)recent fetishisation of `auto` in code.

By all means, if generics are important, and the type can be interpreted easily (e.g. iterators), then that's only a good thing for readiblity, but in most instances where programmers use it like `var` in JavaScript, I find it unecessarily vague and ambiguous when it comes to debugging.

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
#
### Move semantics
Have covered this topic already, but benefits include...
* Avoiding expensive deep cloning operations
* Steering clear from using pointers for objects like Java does
* Staying away from executing error-prone swapping operations which might sacrifice readability

### Swapping
> _"Before move semantics were added in C++11, swapping the content of two objects was a common way to transfer data without allocating and copying"_ – pg. 25

> _"Moving objects only makes sense if the object type owns a resource..."_ – pg. 25

### Rule of 5
Before move semantics, this was usually referred to as the rule of 3:
* Copy constructor
* Copy-assignment operator, and
* Destructor

With move semantics, we now have:
* Move constructor
* Move-assignment operator

These are marked as `noexcept` _"because, as opposed to copy constructor / copy-assignment operator, they do not allocate memory or do something that might throw exceptions"_ pg. 28

My only criticisms on this section would be the lack of protection against self-assignment in copy-/move-assignment operations, and the addition of a preference to using std::exchange over std::move on non-class members.

[rule_of_five.cpp](rule_of_five.cpp)
#
### Named variables an rvalues
These were covered extensively in C++ Primer, but are instrumental in taking advantage of copy and move semantics.
#
### Default move semantics and the rule of zero
Sometimes it's easier to allow the compiler to synthesise its own constructor / assignment operators, although it's not always the best idea.

> _"It's easy to forget that adding just one of the five functions prevents the compiler from generating the other ones. The following version of the Button class has a custom destructor. As a result, the move operators are not generated, and the class will always be copied:"_ – pg. 32

#
### A common pitfall - moving non-resources
In instances where a simple type is mixed with a resource-owning type, std::swap can very useful in avoiding undefined behaviour when implementing move constructors / move-assignment operators, (although, again, be mindful protecting against self-assignment).

[menu.cpp](menu.cpp) | [widget.cpp](widget.cpp)
#
### Applying the && modifier to class member functions
This is quite clever - similar to const, we can create specific overloads for lvalue and rvalues.

[foo.cpp](foo.cpp)
#
