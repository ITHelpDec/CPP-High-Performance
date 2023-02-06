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

#
### ...work in progress
