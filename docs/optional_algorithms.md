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

## Storage Semantics

Optionals have value semantics rather than pointer semantics.
There is no need for manual memory management for optionals,
as they do not use dynamic allocation.
When you pass an optional to a function or assign it to another variable,
you get an new copy.

## Optional Macros O(1)

- `SET_OPTIONAL(optional, item)` stores the item in the optional.
  It sets the optional's count to one.

- `GET_OPTIONAL(optional)` returns the item in the optional.
  It assumes that the optional is not empty.

- `GET_OPTIONAL_OR(optional, default_item)` returns the item in the optional,
  if it is not empty, and otherwise the default item is returned.

- `CLEAR(optional)` decreases count to zero.
  This works the same way for optionals as for dynamic arrays in general.

- `IS_EMPTY(optional)` checks if the optional has a count of zero.
  This work works the same way for optionals as for ranges in general.

- `FOR_EACH(iterator, optional)` is the prefered way to both check if an
  optional contains an item and if so access it.
  This works the same way for optionals as for ranges in general.
  For optionals it results in a "loop" that is run zero or one times.
  If the optional is empty then it runs zero times.
  If the optional contains an item then it runs once
  and the loop variable with the name given by `iterator` will point to
  the item in the optional. Example:

```c
FOR_EACH(it, optional_selected_character) {
    printf("Selected character: %c", *it);
}
```
