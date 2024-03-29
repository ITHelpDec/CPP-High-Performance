# C++ High Performance (2nd Edition)

## Highlights from Chapter 12 - "Coroutines and Lazy Generators"

### Coroutine Example
A quick example of how `std::iota` can be represented with a generator and some co-routine techniques.

Kudos to the author for including a header file that checks for `<experimental/coroutine>` as well as `<coroutine>` - this was a nice touch, and necessary in order to run the code on Xcode.

[coro_iota.cpp](coro_iota.cpp)
#
### Coroutines vs Subroutines
> _"coroutines are subroutines that also can be suspended and resumed. Another way to look at it is to say that subroutines are a specialization of coroutines that cannot be suspended or resumed."_ – pg. 390
#
### CPU Registers
A nice attempt at teaching pseudo-Assembly, although I wish they'd just used Assembly instead of moxing up the commands and "source / destination" ordering from Intel and AT&T syntaxes:
```diff
- add     73, R1
+ addl    $73, %eax (AT&T)
+ add     eax, 73   (Intel)

- mov     SP,  R2
+ movl    -8(%rbp), %eax            (AT&T)
+ mov     eax, dword ptr [rbp - 8]  (Intel)

- mov     R2, [R1]
+ movl    %eax, -8(%rbp)             (AT&T)
+ mov     dword ptr [rbp - 8], eax   (Intel)
```
#
### Call frames
When a function is being called, a __*call frame*__ for that function is created, containing the following:
- parameters
- local variables
- a snapshot of the registers
- a return address that links back to the place in memory where the call was invoked
- an optional frame pointer

A call frame stored on the stack is called a __*stack frame*__.
#
### Stackful vs Stackless
> _"Stackful coroutines have a separate side stack (similar to a thread) that contains the coroutine frame and the nested call frames. This makes it possible to suspend from nested call frames:"_ – pg. 396

> _"the coroutine itself needs to create the coroutine frame and copy the parameters and registers from the call frame to the coroutine frame when it is called:"_ – pg. 396

> _"stackless coroutines use the stack of the currently running thread to handle nested function calls. The effect of this is that a stackless coroutine can never suspend from a nested call frame."_ – pg. 398

#
### Stackful Coroutines vs Threasds
Each have their own stack - hwoever,...:
* Switching threads is a kernel operation, and kernel operations are expensive (pg. 393)
* Most OS's switch threads premptively, as opposed to cooperatively like in coroutines.

#
### Performance
Stackful coroutines are dynamically-allocated, and can either be segmented or expanding (like how `std::vector` grows).

Stackless coroutines do not allocate memory dynamically

The memory footprint im summary:
* __*Stackless*__ – Coroutine frame
* __*Stackful*__ - Coroutine frame + call stack

#
### Context switching
It's generally more expensive to switch between stackful coroutines than it is stackless.
> _"...a stackful coroutine has a more expensive context switch operation since it has more information to save and restore during suspend and resume compared to a stackless coroutine. Resuming a stackless coroutine is comparable to a normal function call."_ – pg. 401

#
### Coroutines in C++
Coroutines in C++ are stackless.

They offer very little memory overhead, efficient context switching, high flexibility (more than 15 customisation points), and don't require C++ exceptions to handle errors (this makes them potentially very useful for the likes of embedded systems or real-time / low-latency environments).

#
### Keywords
* `co_await` - suspend the current coroutine
* `co_yield` - spit out a value and suspend the coroutine
* `co_return` - complete the coroutine (can also return value)

* `std::coroutine_handle` - template class that refers to the coroutine's state (allows for suspending and resuming)
* `std::suspend_never` - trivial awaitable type that never suspens
* `std::suspend_always` - trival awaitable type that always suspends
* `std::coroutine_traits` - defines promise type

#
### Restrictions
* Can't use variadic arguments
* Can't use `auto` (thank goodness..)
* Can't be `constexpr` (maybe less good)
* Constructors / destructors can't be coroutines (and neither can `main()`)

We cannot access the coroutine state by any other means than through the handle (the coroutine state also contains the __*promise*__).

Values or errors from the coroutine passed to the promise – it acts like a conduit between the coroutine and the handler, but (oddly) neither have direct access.

#
## Our first coroutine example
A bit convoluted in how the code was presented in the book, but a good example of what boilerplate is needed to make the coroutine work, as well as what a coroutine actually is!

Basically, it's like having more than one `main()` function.

In real-world terms, it's like "put the kettle on, grind your coffee beans, put your grinds in the french press, go back to the kettle for the boiled water, then go back to the french press to start pouring your water" - the coroutines allow us to keep track of both progress and state  i.e. the grinds don't turn back into beans when we pick up the kettle and wonder why the water unboiled the moment we stopped looking at it 😅

Powerful stuff.

[coro_printVec.cpp](coro_printVec.cpp)

#
### Passing our coroutine around
We can use threads alongside our coroutines - this can help avoid potential heap allocations of the coroutine dtate on a specific thread.

[co_thread.cpp](co_thread.cpp)

#
### Allocating the coroutine state
In some cases, the separate heap allocation can be elided by inlining the coroutine state into the frame of the caller (this is, however, not guaranteed).

> _"For the compiler to be able to elide the heap allocation, the complete lifetime of the coroutine state must be strictly nested within the lifetime of the caller. In addition, the compiler needs to figure out the total size of the coroutine so that parts of it can be inlined. Situations like virtual function calls, and calls to functions in other translation units or shared libraries, typically make this impossible. If the compiler is missing the information it needs, it will insert a heap allocation"_ – pg. 412

We can create a class-level `operator new` to see if the heap elision was successful (and if it wasn't, then we can find out hownmuch memory is needed for the coroutine state).

```cpp
struct Promise {
    // ...
    static void* operator new(std::size_t sz) {
        std::cout << "Promise::operator new(" << sz << ")\n";
        return ::operator new(sz);
    }
    
    static void operator delete(void *mem) {
        std::cout << "Promise::operator delete(mem)\n";
        return ::operator delete(mem);
    }
};
```
#
### Avoiding dangling references
Forget everything you knew about passing `const T &t` - this will result in undefined behaviour (even in lambdas).

Pass by value (seems expensive).
```cpp
// coroutine
Resumable coroutine(std::string s) {
    std::cout << s;
    co_return;
}

// function
Resumable coro_factory() {
    std::string s = "woof\n";
    Resumable res = coroutine(s);
    return res;
}

int main()
{
    Resumable coro = coro_factory();
    
    coro.resume();
    
    return 0;
}
```
#
### Member functions and lambdas
We need to be especially careful of scope when it comes to including coroutines inside classes / structs - same goes for lambdas.

Take advantage of `operator()` to form a lambda member coroutine, or `auto ...` + `...) -> T { ...` for a standalone lambda coroutine - just, again, be mindful of lifespan.

[co_member.cpp](co_member.cpp) | [co_lambda.cpp](co_lambda.cpp)

#
### Generators
> _"A generator is a type of coroutine that yields values back to its caller"_ – pg. 419
* `Generator` - the return object
* `Promise` - coroutine controller
* `Iterator` - interface between client and `Promise`

[generator.cpp](generator.cpp)

#
### Lerp derp
Some nice examples of eager vs lazy evaluation, based off a function using C++20's `std::lerp`:
* [Eagerly generate and return all values](eager.cpp)
* [Using a callback (lazy)](callback.cpp)
* [Using a custom iterator (lazy)](iterator.cpp)
* [Using the Ranges library (lazy)](lazy_range.cpp)
* [Using coroutines with our Generator class (lazy)](lazy_coro.cpp)

Next rant - it's all well and good providing examples of this eager and lazy evaluation, but if we say...
> _"This implementation is very efficient."_ 

...on page 429 for the custom iterator, then it would help to have a visible benchmark of how and why it's more efficient.

I'll probably do one anyway, but the point still stands – make the how and why clear.

#
### Takeaways
C++ is by default an "eager" language, so writing lazily (like in the "callback" version) may be odd to some.

These patterns, however, are commonplace in asynchronous code (which we will cover in the next chapter!), where coroutines can wrap (or even replace!) those callback-based API's.

#
### Gap encoding / decoding
A nice exampple of how generators can be used to create a compression algorithm, similar to what might be used in search engines as part of a data structture called an inverted index.

[gap_encode_decode.cpp](gap_encode_decode.cpp)

We can then use __*variable byte encoding*__ so that smaller gaps are encoded with fewer bytes than larger gaps (switched off reading the code though - still haven't seen how it's more efficient than eager evluation...code attached below).

[variable_byte.cpp](variable_byte.cpp)

#
### Tips for performance
This was mentioned earlier, but immediatley passing a coroutine to another thread is a quick hack to avoid memory on the heap.

> _"However, I will not provide you with any benchmarks of coroutines in this book"_ – pg. 438

This is a total buzzkill – at the time of editing, it's proven difficult benchmarks of lazy vs eager evaluation.

> _"Coroutines that execute on the same thread can share state without using any locking primitives and can therefore avoid the performance overhead incurred by synchronizing multiple threads"_ – pg. 438

#
### Summary
Interesting chapter.

It will be interesting to see how I might implement coroutines into programmes that would benefit from lazy evaluation over eager evaluation.

At the very least, it'll encourage me to try take advantage of more lazy evaluation techniques like with proxy objects from before.

Again, though...still not enough "why?" and "how?" when it comes to tangible and visible performance benefits.
#
### If you've found anything from this repo useful, please consider contributing towards the only thing that makes it all possible – my unhealthy relationship with 90+ SCA score coffee beans.

<a href="https://www.buymeacoffee.com/ITHelpDec"><img src="https://img.buymeacoffee.com/button-api/?text=Buy me a coffee&emoji=&slug=ITHelpDec&button_colour=FFDD00&font_colour=000000&font_family=Cookie&outline_colour=000000&coffee_colour=ffffff" /></a>
