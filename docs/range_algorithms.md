## Range Definition

A **range** is an array of items that cannot grow in size.
A **range** is defined structurally as any struct that has at least
the member variables `data` and `count`.
The member variable `data` should point to an array of `count` items,
where `data` can be any pointer type and `count` can be any integer type.
For example:

```clike
typedef struct {
    int* data;
    size_t count;
} ExampleRange;
```

## Memory Handling

- `INIT_RANGE(range, count)` allocates memory to hold `count` items
  for the given range.

- `FREE_RANGE(range)` frees the memory of the range.
  Sets its count and data pointer to zero.

## Range Macros O(1)

- `IS_EMPTY(range)` checks if the range has a count of zero.

- `FIRST_ITEM(range)` returns the item at index `0`. It assumes that the range is not empty.

- `LAST_ITEM(range)` returns the item at index `count-1`. It assumes that the range is not empty.

## Range Loop Macros O(count)

- `FOR_EACH(iterator, range)` can be used instead of a normal for-loop
  to loop over all items in the `range`.
  A loop variable with the name given by `iterator` will be allocated,
  in the scope inside the loop, and it will point to each item in the range.
  Example:

```c
FOR_EACH(it, range) {
    printf("%i ", *it);
}
```

- `FOR_EACH2(iterator0, iterator1, range0, range1)` can be used instead of a
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

- `FOR_INDEX(index, range)` can be used instead of a normal for-loop
  to loop over all indices of a `range`.
  Example:

```c
FOR_INDEX(i, range) {
    printf("index %i has item %i ", i, range.data[i]);
}
```

- `FOR_EACH_BACKWARD(iterator, range)` can be used instead of a normal for-loop
  to loop over all items in the `range` in reverse order.
  A loop variable with the name given by `iterator` will be allocated,
  in the scope inside the loop, and it will point to each item in the range.
  Example:

```c
FOR_EACH_BACKWARD(it, range) {
    printf("%i ", *it);
}
```

- `FOR_EACH_BACKWARD2(iterator0, iterator1, range0, range1)` can be used instead of a
  normal for-loop to loop over the pairwise items in two ranges in reverse order.
  It is similar to a zip-function.
  Two loop variables with the name given by `iterator0` and `iterator1` will be
  allocated, unfortunately in the scope surrounding the loop,
  and they will point to the pairwise items in the two ranges.
  Example:

```c
FOR_EACH_BACKWARD2(a, b, range0, range1) {
    *a = *b;
}
```

- `FOR_MIN(iterator, range)` find the min item in the range and do something with it.
  The body will be run once, if the min item exists,
  and zero times if it does not exist since the range is empty.
  A loop variable with the name given by `iterator` will be allocated,
  in the scope surrounding the loop.
  It will point to the min item in the range, if it exists.
  Otherwise, it will point to the end of the range.
  Example:

```c
FOR_MIN(it, range) {
    printf("The minimum value in the range is %i ", *it);
}
```

- `FOR_MAX(iterator, range)` find the max item in the range and do something with it.
  The body will be run once, if the max item exists,
  and zero times if it does not exist since the range is empty.
  A loop variable with the name given by `iterator` will be allocated,
  in the scope surrounding the loop.
  It will point to the max item in the range, if it exists.
  Otherwise, it will point to the end of the range.
  Example:

```c
FOR_MAX(it, range) {
    printf("The maximum value in the range is %i ", *it);
}
```

- `ARE_EQUAL(range0, range1)` checks if the two ranges are equal or not.
  Returns `true` or `false`.
  Equality is so far only defined for ranges of primitive types.

- `FILL(range, value)` sets all the items in the `range` to `value`.

- `COPY(source_range, target_range)` overwrites all items in the `target_range` with the correposnding item from the `source_range`.
  Iterates from the start to the end of each range, which only matters if the source and target overlap.

- `COPY_BACKWARD(source_range, target_range)` overwrites all items in the `target_range` with the correposnding item from the `source_range`.
  Iterates from the end to the start of each range, which only matters if the source and target overlap.
