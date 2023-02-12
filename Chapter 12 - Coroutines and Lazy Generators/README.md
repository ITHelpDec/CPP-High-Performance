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
+ add     eax, 73   (Tntel)

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
### ...work in progress
