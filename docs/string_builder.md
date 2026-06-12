## Strings

Carma generally provides macros that you can use on the array structs
that you define on your own. However, Carma makes an exception for
strings and define two concrete array structures that you can use:
`StringView` and `StringBuilder`. They are defined in `carma_string.h`


## StringBuilder

You can use `StringBuilder` for dynamic strings that can grow or shrink in size:

```c
typedef struct StringBuilder {
    char* data;
    size_t count;
    size_t capacity;
} StringBuilder;
```

A `StringBuilder` is both a range and a dynamic array,
so all range and dynamic array macros can be used for it.


## String Macros O(N)

- `read_text_file(file_path_cstring)` reads a text file
  and returns a `StringBuilder` of its content.

- `MAKE_CSTRING(string_builder)` takes a `StringBuilder` and allocate a new copy of it as a c string which is returned:
  Example:
```c
StringBuilder a = read_text_file("data.txt");
char* b = MAKE_CSTRING(a);
b[0] = '\n';
```


## Serialization

These macros serialize / convert a value to a string.
They all take a `StringBuilder` as input and a value and adds the serialized value at the back of the `StringBuilder`.
A null character is always added at the end so that `StringBuilder.data` can be interpreted as a standard c-null-terminated string as well. The null character is hidden in the sense that `StringBuilder.count` does not count it.

- `SERIALIZE_INTEGRAL(string_builder, x)` serializes any integral value `x` to the back of `string_builder`.
  This works for all integral types like:
  `int`, `long`, `char`, `signed` and `unsigned` versions, and aliases like `size_t`.
  Example:
```c
StringBuilder s = {};
SERIALIZE_INTEGRAL(s, 4);
SERIALIZE_INTEGRAL(s, -3);
```

- `SERIALIZE_DOUBLE(string_builder, x)` serializes a `double` or `float` to the back of `string_builder`.
  If the number has a decimal part then it is serialized with 6 decimals.
  Example:
```c
StringBuilder s = {};
SERIALIZE_DOUBLE(s, 1.0);
SERIALIZE_DOUBLE(s, 3.14f);
```

- `SERIALIZE_BOOL(string_builder, x)` serializes anything that can be interpreted as a boolean value to `"true"` or `"false"` at the back of `string_builder`.
  Example:
```c
StringBuilder s = {};
SERIALIZE_BOOL(s, true);
SERIALIZE_BOOL(s, false);
SERIALIZE_BOOL(s, 1);
SERIALIZE_BOOL(s, 0);
```

- `SERIALIZE_CHARACTER(string_builder, x)` serializes a `char` to the back of `string_builder`.
  Example:
```c
StringBuilder s = {};
SERIALIZE_CHARACTER(s, 'a');
SERIALIZE_CHARACTER(s, '\n');
SERIALIZE_CHARACTER(s, 'b');
```

- `SERIALIZE_CSTRING(string_builder, x)` serializes a standard-null-terminated-c-string to the back of `string_builder`.
  `x` can have the types: `char*`, `const char*`, `char[N]`.
  Example:
```c
StringBuilder s = {};
SERIALIZE_CSTRING(s, "Hello");
SERIALIZE_CSTRING(s, " ");
SERIALIZE_CSTRING(s, "World");
```
