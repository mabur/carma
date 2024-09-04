# Different Kind of Arrays

Carma does not define any concrete array structs on its own.
Instead, it works on any structs that follow certain conventions for its member variables.

## Range

A **range** is defined as any struct that has a member variable `data` of any pointer type
and a member variable `count` of any integer type:

```clike
struct ExampleRange {
    int* data;
    size_t count;
    ...
};
```

## Dynamic Array

A **dynamic array** is defined as a struct that is a **range**
with the additional member variable `capacity` of any integer type:

```clike
struct ExampleDynamicArray {
    int* data;
    size_t count;
    size_t capacity;
    ...
};
```

## Image

An **image** is defined as a struct that is a **range**
with the additional member variables `width` and `height` of any integer type:

```clike
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
