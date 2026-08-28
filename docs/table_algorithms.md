# Table Macros

Carma support hash tables that look like this:
```clike
typedef struct Item {
    int key;
    char value;
} Item;

typedef struct Items {
    Item* data;
    size_t count;
    size_t capacity;
} Items;

typedef struct Table {
    Items items;
    TableIndices indices;
} Table;
```

The first part of a carma table is `Table.indices`.
It is always of the type `TableIndices` which Carma provides.
It is used under the hood to map from key hashes to array indices.

The second part of a carma table is `Table.items` which is a normal dynamic array, that holds all items in insertion order.
The keys can either be primitive types like integers, or ranges like Carma strings.

You can use the normal range macros on `Table.items` as long as you don't modify the keys or change the size of the table. You should use the following dedicated table macros when you want to modify a table:

- `INIT_TABLE(table, capacity)` can be used to init an empty table, if you know the capacity you need from the start.
  If you don't know what capacity you want then you can just zero initialize the table instead like `(MyTable){}` or `(MyTable){0}`;

- `FREE_TABLE(table)` frees the memory of the table and sets its capacity to zero.

- `CLEAR_TABLE(table)` removes all items from the table, but keeps its capacity. Has a time complexity of O(capacity).


## Tables with Primitive Keys

If you have a table primitive keys, like int, then you should use these macros to modify the table:

- `SET_KEY_VALUE(key, value, table)` sets the `key` to the `value` in the table. This is how you add new data to the table, or update existing data in the table. Its time complexity is O(1) on average. Example usage:

```clike
Table table = {};
SET_KEY_VALUE(99, 'a', table);
SET_KEY_VALUE(35, 'x', table);
```

- `GET_KEY_VALUE(key, value, table)` looks for the `key` in the table. If the `key` is found then `value` will be set to it. Its time complexity is O(1) on average. Example usage:

```clike
int key = 99;
char value = '\0';
GET_KEY_VALUE(key, value, table);
if (value != '\0') {
    printf("I found the key %d in the table.\n", key)
    printf("It contained the value %c.\n", value);
}
```


## Tables with Ranges as Keys

If you have a table with ranges as keys then you should use these macros to modify the table:

- `SET_KEY_RANGE_VALUE(key, value, table)` sets the `key` to the `value` in the table. This is how you add new data to the table, or update existing data in the table. Its time complexity is O(1) on average.

- `GET_KEY_RANGE_VALUE(key, value, table)` looks for the `key` in the table. If the `key` is found then `value` will be set to it. Its time complexity is O(1) on average.
