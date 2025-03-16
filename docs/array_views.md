## Array View Definition

An **array view** points to an array that it does not own.
An **array view** is defined structurally as any struct that has at least
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

You can drop items from the front or back of an array view.
However, unlike a dynamic array you cannot append items to an array view.

# Drop Macros

These macros change the number of items in an array view,
by removing items in the front or back.
They do this without moving any items or reallocating any memory.
They just change the memory interval that the array view points to.
These macros can be used on both ranges and dynamic arrays.

- If you use these macros on a range make sure that you keep a backup range,
  that refers to the full range, so that no memory gets lost.
- Dynamic arrays only support dropping from the back.
  Then there is no need for a backup.
  But dropping from the front is not supported for dynamic arrays.

You can drop single items like this:

- `DROP_FRONT(range)` increments the data pointer by one and decrements the count by one. O(1).

- `DROP_BACK(range)` decrements the count by one. O(1).

These macros drop items from the range based on if they are equal to a specific item.
The time complexity of these macros are O(count):

- `DROP_FRONT_WHILE_ITEM(range, item)` drop items from the front of the range while they are equal to the given item.

- `DROP_BACK_WHILE_ITEM(range, item)` drop items from the back of the range while they are equal to the given item.

- `DROP_FRONT_UNTIL_ITEM(range, item)` drop items from the front of the range until the given item is found.

- `DROP_BACK_UNTIL_ITEM(range, item)` drop items from the back of the range until the given item is found.

These macros drop items from the range based on if they fulfill a predicate function.
A predicate is a function that takes an item and returns truthy or falsy.
The time complexity of these macros are O(count):

- `DROP_FRONT_WHILE(range, predicate)` drop items from the front of the range while they fulfill the predicate function.

- `DROP_BACK_WHILE(range, predicate)` drop items from the back of the range while they fulfill the predicate function.

- `DROP_FRONT_UNTIL(range, predicate)` drop items from the front of the range until they fulfill the predicate function.

- `DROP_BACK_UNTIL(range, predicate)` drop items from the back of the range until they fulfill the predicate function.
