## Strings

Carma generally provides macros that you can use on the array structs
that you define on your own. However, Carma makes an exception for
strings and define two concrete array structures that you can use:
`StringView` and `StringBuilder`.

## StringView

You can use `StringView` for immutable strings,
for example for wrapping string literals:

```c
typedef struct StringView {
    const char* data;
    size_t count;
} StringView;
```

A `StringView` is a range so all range macros can be used for it.

## StringBuilder

You can use `StringBuilder` for dynamic strings:

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

- `STRING_VIEW(cstring)` takes a c string and returns a `StringView` of it.
  Example:

```c
StringView s = STRING_VIEW("Hello World");
```

- `CONCAT_CSTRING(string_builder, cstring)` concatenates a c string
  at the end of the given `StringBuilder`.
  The null terminator is included at the end but not included in the `count`.
  Example:

```c
StringBuilder s = {};
CONCAT_CSTRING(s, "Hello");
CONCAT_CSTRING(s, " ");
CONCAT_CSTRING(s, "World");
```

- `CONCAT_STRING(string_builder, format, ...)` concatenates a formatted string
  at the end of the given `StringBuilder`. Example:

```c
StringBuilder s = {};
CONCAT_CSTRING(s, "Hello World");
CONCAT_CSTRING(s, " Number %d", 99);
```

- `FORMAT_STRING(const char* format, ...)` formats a string
  and returns a `StringView` of it. An internal buffer is re-used
  which means that you don't need to free the memory of the returned `StringView`,
  but it also means that result is only valid until the next call of the function.
  Example:

```c
StringView s = FORMAT_STRING("Number %d", 99);
```
