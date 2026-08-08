## Strings

Carma generally provides macros that you can use on the array structs
that you define on your own. However, Carma makes an exception for
strings and defines a few different concrete array structures that you can use for strings.
They are defined in `carma_string.h`

## Strings with Fixed Capacity

Carma provides a bunch of fixed capacity strings:
`String2`, `String4`, `String8`, `String16`, `String32`, `String64`, `String128`, `String256`, `String512`, `String1024`.
They are defined like:
```clike
typedef struct String64 {
  char data[64];
  size_t count;
} String64;
```
These fixed capacity strings are ranges so all range macros can be used for them.
They are typically used when you want to construct a short and temporary string from some other values:
```clike
String64 file_path = formatString64("%sworld%d.ppm", image_dir_path, room_id);
```
When you call `formatString64` you construct a null terminated c string that is stored in `data`
and `count` is set to the number of characters excluding the null terminator.

These fixed capacity strings behave like primitive types in the sense that:
* They have value semantics and are copied on assignment.
* They have automatic lifetime management.
* The data is stored on the stack.
* They should NOT be freed.

This makes them suitable for short and temporary strings.
