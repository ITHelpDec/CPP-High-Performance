# C++ High Performance (2nd Edition)

## Highlights from Chapter 2 - "Essential C++ Techniques"

### Using auto in function signatures
Controversial opinion here, but whilst helpful in certain instances, I'm not a fan of the (seemingly-)recent fetishisation of `auto` in code.

By all means, if generics are important, and the type can be interpreted easily (e.g. iterators), then that can only a good thing for readiblity, but in most instances where programmers use it like `var` in JavaScript, I find it unecessarily vague and ambiguous when it comes to debugging.

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
### Pass by value when applicable
Whilst we might be able to write a function that covers both copy- and move-assignment operations on page 37, it involves creating a local copy of our argument, which feels like a waste of resources – I would rather use the const lvalue ref and rvalue overloads.
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
### (Named )Return Value Optimisation
We do not need to use std::move() when returning a value from a function - the compiler will optimise this for us (see ["copy ellision"](https://en.cppreference.com/w/cpp/language/copy_elision))
#
### Contracts
Trhee important things exist in the concept of Design by Contract
* Pre-condition     - specifies the _responsibilities of the function caller_
* Post-condition    - specifies the _responsibilities of the function upon returning_
* Invariant         - _a condition that should alwys hold true_

We can use `static_assert()` and the `assert()` macro defined in the `<cassert>` header to help maintain contracts.

We can also include this handy snippet of code in our header files to take advantage of assert's in debug mode, whilst avoiding them entirely in release mode.
```cpp
#ifdef NDEBUG
#define assert(condition) ((void)0)
#else
#define assert(condition) /* implementation defined */
#endif
```
We can use these assumptions to highlight programming errors, and exceptions for the truly exceptional.
#
### Resource acquisition
A nice example of how C++ can acquire and release resources regardless of its success (or failure) – this is especially important with the likes of mutexes, in order to prevent deadlock.

[mutex.cpp](mutex.cpp)
#
### Exceptions
Exceptions have a bit of a bad rep - they have their uses, but in general they are avoided for a few reasons:
* THey increase the size of the binary
* THrowing and catching exceptions is expensive
#
### Lambda functions
There was a strange eapplication of lambdas by reference and lambdas by value on pg. 54
Whilst it might be convincing to capture by value based on the example, it is the programmer's responsibility responsibility to ensure that whatever information is captured has the intended meaning.
Lambdas are typically used best when a function is needed temporarily over a short period of time – assigning it to a variable name like in the examples as opposed to defining it inside the STL algorithm takes away from the temporary nature of the function.
You can capture by value or by reference to different degrees, as well as modify the contents of the trailing return type by using the `mutable` keyword.
#
### Assigning C function pointers to lambdas
Would be useful when working with older or different codebases - just prefix the capture list with a `+`.
```cpp
extern void download_webpage(const char* url, void (*callback)(int, const char*));
download_webpage("http://www.packt.com", +[] (int result, const char *str) { /* do stuff */ } );
```
#
### Lambda types
_"From C++20 lambdas without ~~captures~~ (parameters?) are default-constructible and assignable through the use of `decltype`)"_ – pg. 60
The example given has an empty capture list but is the first of its kind with no parameter list.
```cpp
auto x = [] {};
```
The structure of a lambda is...
```cpp
[capture list](parameter list) -> return type { function body }
```
..., but this may still prove to be a useful technique to know.
#
