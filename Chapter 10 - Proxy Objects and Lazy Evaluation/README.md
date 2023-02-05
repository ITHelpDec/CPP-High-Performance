# C++ High Performance (2nd Edition)

## Highlights from Chapter 10 - "Proxy Objects and Lazy Evaluation"

### Tucking things under the rug
> _"By using proxy objects, we can achieve optimizations under the hood; the resultant code is both optimized and readable."_ – pg 305

#
### Lazy evaluation vs eager evaluation
We covered these topics breifly in [earlier chapters](../Chapter%2006%20-%20Ranges%20and%20Views/README.md#stdviews-are-lazy-evaluated) with views and materialisations.

> _"Lazy evaluation is a technique used to postpone an operation until its result is really needed."_

> _"The opposite, where operations are performed right away, is called eager evaluation."_

> _"In some situations, eager evaluation is undesirable as we might end up constructing a value that is never used."_ – pg. 306

Given the rest of the topics in this book, this feels like a nice introduction into concurrency in C++.

#
### Examples of lazy and eager evaluation
I don't think the book does a great job of explaining the difference, but I'm assuming the use of `std::function<T()>` allows us to delay the loading of an argument into memory compared to the typical `const T &t` style.
```cpp
// eager: calls "t" into memory early
void display(const Picture &profile) { /*...*/ }
display((load("picture.jpg"));
```
```cpp
// lazy: calls "t" as and when it needs it
void display(std::function<Picture()> profile) { /*...*/ }
display( [] () { return load("picture.jpg"); } );
```
#
### Proxy objects
> _"...using proxy objects, you can encapsulate optimizations in your libraries while leaving the interfaces intact"_ – pg. 307

I'm not quite sure about the example provided in the book - when I tested for performance gains from the second function, the improvements definitely weren't in a runtime capacity. The first function was twice as fast as the second on my machine, regardless of the temporary copy.

[bm_strcmp.cpp](bm_strcmp.cpp)

~~Saying that, with optimisations turned on, the same code runs [extremely quickly](https://godbolt.org/z/Ms767n1T3) - perhaps this is what the author means?~~

~~I'll have to do more research.~~

__EDIT:__ Turns out the compiler was optimising out the loop – unfortunately, using `DoNotOptimize` didn't make too much of a difference on my machine (the first function still came out trumps), whereas the second function always seemed to perform better on the likes of GodBolt even with the new code.

#
### Implementing the proxy
Below is an example from the book of how we can redefine operators to improve the efficiency of operations we want to perform __*without*__ veering away from the original front-facing intentions i.e. `(a + b) == c` can still be written as `(a + b) == c` by the developer using the library to test if the concatenation of two strings is the same as another string using the same predictable syntax, but the underlying operations to produce that result can be optimised under the radar without affecting the end result - winner winner, chicken dinner.

[concat_proxy.cpp](concat_proxy.cpp)

#
### Assigning a concatenated proxy
We can add overload `operator String()` within the `ConcatProxy` struct:
```cpp
operator String() const && { return String(a_ + b_); }
```
We must be sure, however, to specifically declare the return type as `String`, and not `auto`, or the programme will assign the result to `ConcatProxy`
```cpp
✅: String c = String("man") + String("bearpig");
❌: auto c = String("man") + String("bearpig");
```
#
### ...work in progress
