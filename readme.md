# What is Carma?

CARMA is a single C header with C ARray MAcros.

It follows the tradition of C developers implementing their own utility
functions to deal with arrays,
since the C standard library is so bare bone.

## Different Kind of Arrays

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

## Range Macros O(1)

* `IS_EMPTY(range)` checks if the range has a count of zero.

* `FIRST_ITEM(range)` returns the item at index 0. It assumes that the range is not empty.

* `LAST_ITEM(range)` returns the item at index count-1. It assumes that the range is not empty.

* `DROP_FIRST(range)` increments the data pointer by one and decrements the count by one. Does not reallocate any memory.

* `DROP_LAST(range)` decrements the count by one. Does not reallocate any memory.

## Range Loop Macros O(count)

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
normal for-loop to loop over the pairwise items in two ranges.
It is similar to a zip-function.
Two loop variables with the name given by `iterator0` and `iterator1` will be
allocated, unfortunately in the scope surrounding the loop,
and they will point to the pairwise items in the two ranges.
Example:
```c
FOR_EACH2(a, b, range0, range1) {
    *a = *b;
}
```

* `FOR_INDEX(index, range)` can be used instead of a normal for-loop
  to loop over all indices of a `range`.
Example:
```c
FOR_INDEX(i, range) {
    printf("index %i has item %i ", i, range.data[i]);
}
```

* `FOR_EACH_BACKWARD(iterator, range)` can be used instead of a normal for-loop
  to loop over all items in the `range` in reverse order.
  A loop variable with the name given by `iterator` will be allocated,
  in the scope inside the loop, and it will point to each item in the range.
  Example:
```c
FOR_EACH_BACKWARD(it, range) {
    printf("%i ", *it);
}
```

* `FILL(range, value)` sets all the items in the `range` to `value`.

## Image Loop Macros

The range loop macros above can also be used for images, since images are ranges.
But Carma also provides some loop macros that only works for images:

* `FOR_X(x, image)` can be used instead of a normal for-loop
to let `x` loop from `0` to `image.width - 1`.

* `FOR_Y(y, image)` can be used instead of a normal for-loop
  to let `y` loop from `0` to `image.height - 1`.

FOR_X and FOR_Y can be used independently,
but they can also be used together like in this example:
```c
FOR_Y(y, image) {
    FOR_X(x, image) {
        printf("(x=%i,y=%i) ", x, y);
    }
    printf("\n");
}
```

## Dynamic Array Algorithms

These algorithms change the count and capacity of a dynamic array:

* `APPEND(dynamic_array, item)` adds item to the end of the dynamic_array.
It increases count by one.
If the new count would be larger than the existing capacity then
the capacity is increased and the memory of the data pointer is reallocated.

* `CONCAT(dynamic_array, range)` adds all items in range to the end of the dynamic_array.
  It increases dynamic_array.count with range.count.
  If the new count would be larger than the existing capacity then
  the capacity is increased and the memory of the data pointer is reallocated.

* `CLEAR(dynamic_array)` decreases count to zero.

* `RESERVE(dynamic_array, capacity)` sets a new capacity and reallocates the data pointer.
If the new capacity is smaller than the old count,
then the new count is set to the new capacity, thereby removing items at the end.

* `ERASE_INDEX(dynamic_array, index)` erases the item at index.
Decreases count by one.
The order of the remaining items is NOT preserved.

* `ERASE_IF(dynamic_array, predicate)` erases all items for which the
predicate function is true.
It reduces the count of the dynamic array accordingly.
The order of the items in the array is NOT preserved.
