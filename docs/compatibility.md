# Compatibility Macros

Carma provides some macros to allow compatibility across compilers and C language dialects,
as long as you have any of these:

- Any compiler supporting GCC or Clang extensions
- Any compiler supporting C23
- Any compiler supporting C++11

In practice that means that these common compilers work:

- GCC 4.9 or newer in C or C++ mode
- Clang 3.3 or newer in C or C++ mode
- MSVC 2013 or newer in C++ mode

## Type Inference

`CARMA_AUTO` is used for type inference, similar to `auto` in C++11 and C23,
and the `__auto_type` extension in GCC and Clang.
It is mainly for internal library usage.

## Aggregate Construction

`MAKE(type, ...)` is used for constructing values of structs, unions and arrays,
in a way that is supported by both C and C++. Example:

```c
// Carma:
MAKE(Vec3, 1.0, 2.0, 3.0)

// Results in this in C:
(Vec3){1.0, 2.0, 3.0}

// Results in this in C++:
Vec3{1.0, 2.0, 3.0}
```

The `MAKE` version is also easier to type on a (Swedish) keyboard,
compared to the raw C version that mixes different kind of braces.

The `MAKE` macro can also be used for designated initializers,
if your compiler supports C99 or C++20:

```C
MAKE(Color, .red=0.5, .green=0.0, .blue=1.0)
```
