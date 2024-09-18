## Image Definition

An **image** is a **range** over two dimensions with a fixed size.
And **image** is structurally defined as a **range**
with the additional member variables `width` and `height` of integer type.
For example:

```c
struct ExampleImage {
    int* data;
    size_t count;
    size_t width;
    size_t height;
};
```

For an image `count` == `width * height`.

## Image Macros

The range loop macros can also be used for images, since images are ranges.
But Carma also provides some loop macros that only works for images:

- `FOR_X(x, image)` can be used instead of a normal for-loop
  to let `x` loop from `0` to `image.width - 1`.

- `FOR_Y(y, image)` can be used instead of a normal for-loop
  to let `y` loop from `0` to `image.height - 1`.

`FOR_X` and `FOR_Y` can be used independently,
but they can also be used together like in this example:

```c
FOR_Y(y, image) {
    FOR_X(x, image) {
        printf("(x=%i,y=%i) ", x, y);
    }
    printf("\n");
}
```
