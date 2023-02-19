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
### ...work in progress
