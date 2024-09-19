# Table Macros

A **table** has fast insertion and lookup, O(1).
It is implemented as a hash table.
The items in a table are structs with the three members:
`key`, `value`, `occupied`.
They should all be primitive types.
This is an example of an item struct for a table:

```c
typedef struct {
    int key;
    char value;
    bool occupied;
} Item;
```

The table then looks just like a dynamic array of such items:

```c
typedef struct {
    Item* data;
    size_t count;
    size_t capacity;
} Table;
```

However, the table is not a dynamic array and you should not use the dynamic array macros on it. You should instead use the following dedicated table macros.

- `SET_KEY_VALUE(key, value, table)` sets the `key` to the `value` in the table. This is how you add new data to the table, or update existing data in the table. Time complexity O(1). Example usage:

```c
Table table = {};
SET_KEY_VALUE(99, 'a', table);
SET_KEY_VALUE(35, 'x', table);
```

- `FIND_KEY(key, value_iterator, table)` looks for the `key` in the table. If the `key` is found then `value_iterator` will point at it. It is semantically similar to a for loop, but with better time complexity O(1). Example usage:

```c
int key = 99;
FIND_KEY(key, value_iterator, table) {
    printf("I found the key %d in the table.\n", key)
    printf("It contained the value %c\n", *value_iterator);
}
```

- `CLEAR_TABLE(table)` removes all items from the table, but keeps its capacity. Has a time complexity of O(capacity).

- `FOR_EACH(table)` can be used to loop over all items in a table, similar to how you can use it for ranges and dynamic arrays. However, when looping over tables you need to make sure to only check the items that are `occupied`. Example:

```c
FOR_EACH(it, table) {
    if (it->occupied) {
      printf("key %d has value %c\n", it->key, it->value);
    }
}
```
