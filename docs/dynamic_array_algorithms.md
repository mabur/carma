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

## Memory Handling

- `INIT_DARRAY(dynamic_array, count, capacity)` allocates memory to hold up to `capacity` items
  for the given `dynamic_array`. The `count` is set to zero.

- `FREE_DARRAY(dynamic_array)` frees the memory of the `dynamic_array`.
  Its count, capacity, and data pointer are set to zero.

- `RESERVE(dynamic_array, capacity)` sets a new capacity and reallocates the data pointer.
  If the new capacity is smaller than the old count,
  then the new count is set to the new capacity, thereby removing items at the end.

## Dynamic Array Macros O(1)

- `APPEND(dynamic_array, item)` adds item to the end of the dynamic_array.
  It increases count by one.
  If the new count would be larger than the existing capacity then
  the capacity is increased and the memory of the data pointer is reallocated.

- `CLEAR(dynamic_array)` decreases count to zero.

- `ERASE_BACK(dynamic_array)` erases the last item.
  Decreases count by one.

- `ERASE_INDEX(dynamic_array, index)` erases the item at index.
  Decreases count by one.
  The order of the remaining items is NOT preserved.

## Dynamic Array Macros O(count)

- `ERASE_FRONT(dynamic_array)` erases the first item.
  Decreases count by one.
  All remaining items are shifted by one index.

- `PREPEND(dynamic_array, item)` adds item to the beginning of the dynamic_array.
  It shifts all previously existing items by one index.
  It increases count by one.
  If the new count would be larger than the existing capacity then
  the capacity is increased and the memory of the data pointer is reallocated.

- `INSERT_INDEX(dynamic_array, index, item)` adds `item` at the given `index` of the `dynamic_array`.
  It shifts all previously existing items, from that index and up to the end of the array, to make place for the new item.
  It increases count by one.
  If the new count would be larger than the existing capacity then
  the capacity is increased and the memory of the data pointer is reallocated.

- `INSERT_RANGE(dynamic_array, index, range)` inserts the `range` at the given `index` of the `dynamic_array`.
  It shifts all previously existing items, from that index and up to the end of the array, to make place for the new item.
  It increases `dynamic_array.count` with `range.count`.
  If the new count would be larger than the existing capacity then
  the capacity is increased and the memory of the data pointer is reallocated.

- `CONCAT(dynamic_array, range)` adds all items in range to the end of the dynamic_array.
  It increases `dynamic_array.count` with `range.count`.
  If the new count would be larger than the existing capacity then
  the capacity is increased and the memory of the data pointer is reallocated.

- `ERASE_INDEX_ORDERED(dynamic_array, index)` erases the item at index.
  Decreases count by one.
  The order of the remaining items is preserved.
  It shifts the remaining items from index up to the end by one.

- `ERASE_MANY_ORDERED(dynamic_array, index, erase_count)` erases `erase_count` neighbouring items starting at `index`.
  The remaining items that comes after are shifted.
  The order of the remaining items is preserved.

- `ERASE_IF(dynamic_array, predicate)` erases all items for which the
  predicate function is true.
  It reduces the count of the dynamic array accordingly.
  The order of the items in the array is NOT preserved.
