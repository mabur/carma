## Dynamic Array Definition

A **dynamic array** is an array that can both grow and shrink at the end of the array.
A **dynamic array** is structurally defined as any struct that is a **range**
with the additional member variable `capacity` of any integer type.
For example:

```c
typedef struct {
    int* data;
    size_t count;
    size_t capacity;
} ExampleDynamicArray;
```

The `capacity` defines the number of items that fit in the array
before its memory needs to be reallocated. So `count` <= `capacity`.

## Dynamic Array Macros O(1)

- `APPEND(dynamic_array, item)` adds item to the end of the dynamic_array.
  It increases count by one.
  If the new count would be larger than the existing capacity then
  the capacity is increased and the memory of the data pointer is reallocated.

- `CLEAR(dynamic_array)` decreases count to zero.

- `ERASE_INDEX(dynamic_array, index)` erases the item at index.
  Decreases count by one.
  The order of the remaining items is NOT preserved.

## Dynamic Array Macros O(count)

- `CONCAT(dynamic_array, range)` adds all items in range to the end of the dynamic_array.
  It increases `dynamic_array.count` with `range.count`.
  If the new count would be larger than the existing capacity then
  the capacity is increased and the memory of the data pointer is reallocated.

- `RESERVE(dynamic_array, capacity)` sets a new capacity and reallocates the data pointer.
  If the new capacity is smaller than the old count,
  then the new count is set to the new capacity, thereby removing items at the end.

- `ERASE_IF(dynamic_array, predicate)` erases all items for which the
  predicate function is true.
  It reduces the count of the dynamic array accordingly.
  The order of the items in the array is NOT preserved.
