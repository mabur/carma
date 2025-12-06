## Optional Definition

An **optional** is a dynamic array that can hold either zero or one value.
An **optional** is defined structurally as any struct that has
the member variables `data` and `count`, where `data` is an array of size one
and `count` can be any integer type. For example:

```c
typedef struct {
    char data[1];
    size_t count;
} OptionalCharacter;
```

## Optional Macros O(1)

- `SET_OPTIONAL(optional, item)` stores the item in the optional.
  It sets the optional's count to one.

- `CLEAR(optional)` decreases count to zero.

- `IS_EMPTY(optional)` checks if the optional has a count of zero.
  Since an optional is a range, this works just the same way for
  optionals as for ranges in general.

- `FOR_EACH(iterator, optional)` is the prefered way to both check if an
  optional contains an item and if so access it.
  It can be seen as a "loop" that is run zero or one times.
  If the optional is empty then it runs zero times.
  If the optional contains an item then it runs once
  and the loop variable with the name given by `iterator` will point to
  the item in the optional. Since an optional is a range,
  this works just the same way for optionals as for ranges in general.
  Example:

```c
FOR_EACH(it, optional_selected_character) {
    printf("Selected character: %c", *it);
}
```
