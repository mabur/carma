CARMA is a single header with C ARray MAcros.

It follows the tradition of developers implementing their own utility
functions to deal with arrays, since the C standard library is a bit lacking in this regard.

# Arrays

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

# Range Loop Macros

* `FOR_EACH(iterator, range)` can be used instead of a normal for-loop 
to loop over all items in the `range`.
A loop variable with the name given by `iterator` will be allocated,
in the scope inside the loop, and it will point to each item in the range.
Example:
```c
FOR_EACH(it, range) {
    printf("%i ", *it);
}
```

* `FOR_EACH2(iterator0, iterator1, range0, range1)` can be used instead of a
normal for-loop to loop over all items in two ranges.
Two loop variables with the name given by `iterator0` and `iterator1` will be
allocated, unfortunately in the scope surrounding the loop, and they will point to each item in the two ranges.
Example:
```c
FOR_EACH2(a, b, range0, range1) {
    *a = *b;
}
```
* `ENUMERATE(index, range)` can be used instead of a normal for-loop
  to loop over all indices of a `range`.
Example:
```c
ENUMERATE(i, range) {
    printf("index %i has item %i ", i, range.data[i]);
}
```

# Dynamic Array Algorithms

* `APPEND(dynamic_array, item)` adds `item` to the end of the `dynamic_array`.
It increases `count` by one.
If the new `count` would be larger than the existing `capacity` then
the `capacity` is increased and the memeroy of the `data` pointer is reallocated.

* `CLEAR(dynamic_array)` decreases `count` to zero.

* `ERASE_INDEX(dynamic_array, index)` erases the item at `index`.
Decreases `count` by one.
The order of the remaining items is NOT preserved.

* `ERASE_IF(dynamic_array, predicate)` erases all items for which the
`predicate` function is `true`.
It reduces the `count` of the dynamic array accordingly.
The order of the items in the array is NOT preserved.
