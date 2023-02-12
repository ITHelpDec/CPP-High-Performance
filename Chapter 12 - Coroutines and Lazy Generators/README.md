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
### ...work in progress
