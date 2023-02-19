# C++ High Performance (2nd Edition)

## Highlights from Chapter 13 - "Asynchronous Programming with Coroutines"

### Awaitable types (revisited)
These were touched on briefly [earlier](../Chapter%2012%20-%20Coroutines%20and%20Lazy%20Generators/README.md#keywords), but here we expand on the topic in a little more detail.

[co_await.cpp](co_await.cpp)

#
### Implicit suspend points
Each coroutine has two implicit suspend points:
* The __*initial suspend point*__, and
* The __*final suspend point*__

The promise type defines the behaviour of these two points, and it is typical to pass `std::suspend_always()` from the initial suspend.

#
### Implementing a rudimentary `Task` type
The aim is to write asynchronous code like this:
```cpp
auto image = co_await load("image.jpg");
auto thumbnail = co_await resize(image, 100, 100);
co_await save(thumbnail, "thumbnail.jpg");
```
In other words, "we want to plaster co_await everywhere like we do with auto".

`std::future` blocks threads when retrieving values, so it does not support continuations (pg. 443), although apparently `std::experimental::future` does (which `experimental` across compilers, who knows). It also creates allocations on the heap and include synchronisation primities, so we're better off creating our own type.

The code is on the author's [GitHub](https://github.com/PacktPublishing/Cpp-High-Performance-Second-Edition/blob/master/Chapter13/task.h) if you're interested - similar to other examples given in previous chapters, the example from the book at this stage is incomplete, so (whilst it might be a nice exercise in typing out the code) not being able to visualise and tweak a finished product is irritating (I promise I'll say nice things eventually!).

#

### Handling errors
We can use a `std::variant` within the `Promise` class to handle the three possible outcomes:
* No value at all - `std::monostate`
* A return value of type `T` - `T`
* An exception - `std::exception_ptr`

```cpp
std::variant<std::monostate, T, std::exception_ptr> result_;
```
>_"The exception is captured by using the std::current_exception() function inside the function Promise::unhandled_exception()."_<br>
> _"By storing a std::exception_ptr, we can later rethrow this exception in another context."_<br>
> _"This is also the mechanism used when exceptions are passed between threads."_<br>
> – pg. 445

__*NB:*__ Implementing a promise type that contains both `return_void()` and `return_value()` generates a compilation error.

#
### Symmetric transfer
We can use a technique called __*symmetric transfer*__ to avoid creating nested stack frames during continuation.
```cpp
auto await_suspend(std::experimental::coroutine_handle<Promise> h) noexcept
{   // symmetric transfer
    return h.promise().continuation_;
}
```
> _"An optimization called tail call optimization is then guaranteed to happen by the compiler."_<br>
> "_In our case, this means that the compiler will be able to transfer control directly to the continuation without creating a new nested call frame."_ – pg. 448

#
### Synchronously waiting for a task to complete
> _"Once we call an asynchronous function that returns a Task, we need to co_await on it, or nothing will happen._"<br>
> _"This is also the reason why we declare Task to be nodiscard: so that it generates a compilation warning if the return value is ignored, like this:"_ – pg. 450

#
### Implementing `sync_await()`
Another function from [cppcoro](https://github.com/lewissbaker/cppcoro#sync_wait) that is jsut copy-pasta with no "this is why you want to use this feature / look at the improvement!".

> _"Destroying a coroutine must only happen if the coroutine is in a suspended state."_ – pg. 454

#
### The end result...
```cpp
Task<int> height() { co_return 20; }
Task<int>  width() { co_return 30; }
Task<int>   area()
{
  co_return co_await height() * co_await width();
}

int main()
{ // Dummy coroutines
  Task<int> a = area();
  int value = sync_wait(a);
  std::cout << value;
}
```
Three pages of boilerplate...to calculate an area...

#
### `boost::asio`
Just because recruiters want to see use of boost.

[boost_asio.cpp](boost_asio.cpp)

### ...work in progress
