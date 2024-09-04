# Different Kind of Arrays

Carma does not define any concrete array structs on its own.
Instead, it works on any structs that follow certain conventions for its member variables.

A **range** is defined as any struct that has a member variable `data` of pointer type
and a member variable `count` of integer type:

```c
struct ExampleRange {
    int* data;
    size_t count;
    ...
};
```

A **dynamic array** is defined as a struct that is a **range**
with the additional member variable `capacity` of integer type:

```c
struct ExampleDynamicArray {
    int* data;
    size_t count;
    size_t capacity;
    ...
};
```

An **image** is defined as a struct that is a **range**
with the additional member variables `width` and `height` of integer type:

```c
struct ExampleImage {
    int* data;
    size_t count;
    size_t width;
    size_t height;
    ...
};
```

So an **image** is a **range** but not a **dynamic array**,
since it does not have a `capacity`.
