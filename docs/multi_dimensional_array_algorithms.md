## 2D Array Definition

A **2D Array** is a **range** with the additional member variables
`width` and `height` of integer type. For example:

```c
typedef struct Image {
    int* data;
    size_t count;
    size_t width;
    size_t height;
} Image;
```

For a 2D array `count` == `width * height`.

## 3D Array Definition

A **3D Array** is a **range** with the additional member variables
`width` and `height` and `depth` of integer type. For example:

```c
typedef struct Voxels {
    int* data;
    size_t count;
    size_t width;
    size_t height;
    size_t depth;
} Voxels;
```

For a 3D array `count` == `width * height * depth`.

## Multi Dimensional Array Macros

The range loop macros can also be used for multi demensional arrays,
since they are ranges.
But Carma also provides some loop macros that only works for multi dimensional arrays:

- `FOR_X(x, array)` can be used instead of a normal for-loop
  to let `x` loop from `0` to `array.width - 1`.

- `FOR_Y(y, array)` can be used instead of a normal for-loop
  to let `y` loop from `0` to `array.height - 1`.

- `FOR_Z(z, array)` can be used instead of a normal for-loop
  to let `z` loop from `0` to `array.depth - 1`.

`FOR_X` and `FOR_Y` and `FOR_Z` can be used independently,
but they can also be used together like in this example:

```c
FOR_Y(y, image) {
    FOR_X(x, image) {
        printf("(x=%i,y=%i) ", x, y);
    }
    printf("\n");
}
```

- `AT_XY(array, x, y)` looks up the item with coordinates (x,y) in a 2D array.
  This corresponds to the index `x + y * width`.

- `AT_XYZ(array, x, y, z)` looks up the item with coordinates (x,y,z) in a 3D array.
  This corresponds to the index `x + y * width + z * width * height`.
