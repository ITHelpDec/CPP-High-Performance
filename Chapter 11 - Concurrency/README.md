# C++ High Performance (2nd Edition)

## Highlights from Chapter 11 - "Concurrency"

### Why?
* Many hands make light work
* We can offload work on to other cores to preserve UI functinality

#
### Challenges
* Sharing memory (dataraces, deadlocks, etc.)
* More complicated / harder to debug

#
### Concurrency ≠ Parallelism
* __Concurrent__

> _"A program is said to run concurrently if it has multiple individual control flows running during overlapping time periods"_ – pg. 327

* Parallel

> _"The threads may or may not execute at the exact same time, though. If they do, they are said to execute in parallel"_ – pg. 327

#
### Shared memory
> _"Threads created in the same process share the same virtual memory."_ – pg. 329

> _"Virtual memory only protects us from accessing memory allocated in different processes to our own."_ – pg. 329

> _"We should always strive to minimize the number of shared resources between threads."_ – pg. 329

> _"Each thread has its own stack for storing local variables and other data necessary for handling function calls. 
Unless a thread passes references or pointers to local variables to other threads, no other thread will be able to access the stack from that thread. 
This is one more reason to use the stack as much as possible"_ – pg. 329

This last quotation is one of the most salient points – where reasonable, __use the stack__.

#
### Thread Local Storage (TLS)
Thread local storage can be used to _"store variables that are global in the context of a thread but which are not shared between threads."_

Everything else is shared by default i.e. dynamically-allocated memory on the heap, global variables, static local variables.

> _"Whenever you have shared data that is mutated by some thread, you need to ensure that no other thread is accessing that data at the same time or you will have a data race"_ – pg. 330

#
### Data races
> _"A data race happens when two threads are accessing the same memory at the same time and at least one of the threads is mutating the data."_ – pg. 331

> _"If your program has a data race, it means that your program has undefined behavior."_ – pg. 331

Data races = no bueno.

Bar undefined behaviour, they can lead to corrupt values as a result of __*tearing*__ (torn reads / torn writes)

No code was provided of a data race to accompany this subheading, so I've put together a (probably incorrect) programme demonstrating serial execution, a threaded data race, and a threaded version that uses mutexes (again, probably incorrectly).

[bm_threads.cpp](bm_threads.cpp)

I've also attached a link to a benchmark on quick-bench [here](https://quick-bench.com/q/v54AIi8eNp7tkMVM-Jq13-Ejvrc) – even if we split our `increase()` and `decrease()` functions across two threads, performance starts to really improve beyond 100,000 iterations.

Both threaded versions perform at around the same speed, but the one without mutexes exhibits undefined behaviour returning muddled values for `x`:
```
x: 9973
x: 18446744073709244579
x: 18446744073707368975
x: 18446744073679433170
```
Our mutex version, however, returns `x` as 0 every time, just as we saw and expected with the initial version that follows serial execution.

#
### Avoiding data races
* Atomic types (e.g. `std::atomic<int>`)
* Mutexes (e.g. `std::mutex`)

A mutually-exclusive lock (mutex) guarantees that multiple threads never execute a critical section at the same time.

Immutable data structures can be accessed concurrently without the risk of incurring a data race.

A common pattern is also to create a new data structure concurrently, then allow it to replace the original data structure instead of modifying the original structure concurrently. That swap is a critical section, so we would use an atomic operation or a mutex to protect it.

#
### Mutexes
A mutex guarantees that only one thread at a time is inside a critical section.

It is worth noting that if most of the work cannot be done without serialisation, then it's not worth writing concurrently.

The state where one thread is blocked by another is called __*contention*__ - this is something we strive to minimise, as adding more cores will not improve performance where contention is high.

#
### Deadlocks
[Hire me and I'll explain deadlocks.](https://vm.tiktok.com/ZMY2WhuD2/)

#
### Threads
We can print the main thread's identifier as such:
```cpp
std::cout << std::this_thread::get_id() << '\n'; // 0x1f38b8100
```
We can use sleep functions to help debug data races
```cpp
std::this_thread::sleep_for(std::chrono::seconds(3));
```

#
### Joining threads
Divide and conquer...just remember to reconvene or you're crash your programme.

[join.cpp](join.cpp)

#
### Thread count
We can get the thread count of a machine by making the following call:
```cpp
std::cout << "Thread count: " << std::thread::hardware_concurrency() << '\n'; // 12
```
#
### `std::jthread`
C++20 introduced a joinable thread to remove the need to supply a token manually - again though, not supported across the board (it doesn't even run properly [on Godbolt...](https://godbolt.org/z/zaY9M4nhK))

[jthread.cpp](jthread.cpp)

#
### Protecting critical sections
I've amended [bm_threads.cpp](bm_threads.cpp) to use `std::scoped_lock` from the example instead of `std::guard_lock`.

#
### Avoiding deadlocks
Sometimes it's necessary to acquire another lock while already holding onto a previously acquired lock - we can do this using `std::lock()`.

I've attached an example from the book of how that might look transferring money concurrently from one account to another.

[money_transfer.cpp](money_transfer.cpp)

#
### Condition variables
> _"A condition variable makes it possible for threads to wait until some specific condition has been met"_ – pg. 346

We then have __*consumers*__ and __*producers*__ (which makes me think of the [Actor](https://www.actor-framework.org) framework).

There is actually a really good example from the book of how two threads can communicate with each other using condition variables and unique locks – here the consumer rests idly, waiting for the producer to perform an action before it performs its own action.

My favourite part about this is how CPU usage is negligible while the consumer thread waits for its notification from the producer.

This technique can be performed using a `while` loop, or by using a specific overload of `wait` that returns the `while` condition in a lambda – both are below.

https://github.com/ITHelpDec/CPP-High-Performance/blob/f0e14bb5310b403577c6f0870c2d765154088ab8/Chapter%2011%20-%20Concurrency/wait_overload.cpp#L18-L19

[condition_variables.cpp](condition_variables.cpp) | [wait_overload.cpp](wait_overload.cpp)

#
### Futures and promises
Futures and promises allow us to return data and handle errors concurrently.
- The `future` is the receiving end of the value, and
- The `promise` is the returning end of the value

[promises.cpp](promises.cpp)

The nice thing about this example is that we didn't need to share global data or call explicit locks - very handy.

> _"...when the value is needed by some client, it calls get() to get hold of the actual value. If it is not computed at that point in time, the call to get() will block until it is finished"_ – pg. 349

#
### Tasks
We can benefit from higher-level abstraction as a codebase grows by taking advantage of `std::packaged_task`, although I notice that if the `std::future` is not called before the `std::thread`, we encounter a runtime error (this was not the case with `std::promise` – will have to see how this pans out over time when I start applying it to larger sections of code.

[task.cpp](task.cpp)

#
### `std::async`
Threads are not free – the number of threads in our programme can affect performance.

With `std::async` we can move from thread-based programming to task-based programming (why does this sound like another Javascript-ism?).

[async.cpp](async.cpp)

#
### C++20 primitives
* __*Latches*__
  * Basically the "ready, set, go!" of multithreading
  * Latches create a synchronisation point where all threads must arrive before cracking on with the rest of their duties.
  * [latch_example.cpp](latch_example.cpp) – (rejigged - I think the author is allergic to whitespace)

* __*Barriers*__
  * If we need multiple synchronisation points then we can opt for a `std::barrier`
  * Again, the equivalent of "ready, set, go!", followed by jumping on a motorbike to the next checkpoint and waiting for all the contestants to congregate before having a little award ceremony and starting the next leg
  * > _"Barriers are useful in parallel programming algorithms that are based on the fork-join model"_ – pg. 355
  * [barriers.cpp](barriers.cpp) – (again, rejigged - I don't understand the fear of whitespace / legible code)

* __*Semaphores*__
  * Think of _semáforo_ in Spanish, or _семафор_in Russian; from _σήμα_ ("sign") and _φορος_ ("bearer") in Greek, this word is can be seen used for "traffic lights", and that's exactly what semaphores do - they provide signals.
  * [counting_semaphore.cpp](counting_semaphore.cpp) - in this example, the semaphore can be default-initialised inline by using `{ 4 }` or in the constructor.
  * [bounded_buffer_1.cpp](bounded_buffer_1.cpp) - first step to creating a push/pop buffer
  * [bounded_buffer_2.cpp](bounded_buffer_2.cpp) – second step including semaphores, although I'm not sure I follow the logic of a pop() function that doesn't remove the last element and move the read position back a step...?

#
### Atomic support in C++
An atomic variable can be safely used and mutated hy multiple threads without introducing daya races.

With atomics, this...
```cpp
int counter = 0;
std::mutex mtx;

void increment(int n) {
    for (int i = 0; i != n; ++i) {
        std::scoped_lock lock(mtx);
        ++counter;
    }
}
```
...becomes this...
```cpp
std::atomic<int> counter;

void increment(int n) {
    for (int i = 0; i != n; ++i) {
        ++counter; // or counter.fetch_add(1);
    }
}
```
Atomics will only work for objects that are copyable, so objects that contain virtual functions or pointers to dynamic memory are off the cards.
#
### Lock-free property
Atomics reduce the performance overhead found in using mutexes, so work very well for low-latency environments.

If an atomic does not use a lock, it is said to be __lock-free__ - we can check this at runtime with...
```cpp
std::atomic<int> x = 1;
assert(x.is_lock_free()); // runtime assert
```
We can also check at compile time as of C++17:
```cpp
static_assert(std::atomic<int>::is_always_lock_free);
```
#
### Atomic flags
We can use atomic flags to protect critical sections of code - they are not very resource-friendly when used in combination with while loops to check state:
```cpp
std::atomic_flag fleg;
fleg.test_and_set();
fleg.test();
fleg.clear(); // they took our flegs
```
Fortunately, as of C++20 we can take advantage of `wait()` and `notify_one()` / `notify_all()` to help reduce spinlock.
#
### Using `std::shared_ptr<T>` in a multithreaded environment
This...
```cpp
std::shared_ptr<int> sp;
```
...becomes...
```cpp
std::atomic<std::shared_ptr<int>> asp;
```
...although, yet again, this is not supported by all compilers...

[atomic_shared_ptr.cpp](atomic_shared_ptr.cpp)
#
### Atomic refs
Speaking of C++20 features that aren't fully supported - introducing `std::atomic_ref` 😃

Have included the example from the book with a few tweaks in regards to pseudo-random numbers, and then the same example written another way.

[coin_game.cpp](coin_game.cpp) | [atomic_ref.cpp](atomic_ref.cpp) | [not_so_atomic_ref.cpp](not_so_atomic_ref.cpp)

#
### C++ Memory Model
The C++ memory model guarantees sequential consistency.

> _"Sequential consistency guarantees that the result of the execution is the same as if the operations were executed in the order specified by the original program."_ – pg. 376

Bit of a weird subsection going into the detail of the model then saying "don't change it" – similar kind of vibe to the Memory Management chapter.

#
### Lock-free programming
Just a few examples from the author - I'll learn more about these concepts when I start reading C++ Concurrency in Action.

[lock_free_queue.cpp](lock_free_queue.cpp)

#
### ...work in progress
