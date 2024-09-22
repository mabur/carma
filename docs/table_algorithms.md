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

- `INIT_TABLE(table, capacity)` can be used to init an empty table, if you know the capacity you need from the start.
  If you don't know what capacity you want then you can just zero initialize the table instead like `(MyTable){}` or `(MyTable){0}`;

- `FREE_TABLE(table, capacity)` frees the memory of the table and sets it's capacity to zero.

- `CLEAR_TABLE(table)` removes all items from the table, but keeps its capacity. Has a time complexity of O(capacity).

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
    printf("It contained the value %c.\n", *value_iterator);
}
```

- `FOR_EACH_TABLE(table)` can be used to loop over all items in a table. Example usage:

```c
FOR_EACH_TABLE(it, table) {
    printf("key %d has value %c\n", it->key, it->value);
}
```
