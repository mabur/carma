# Image Loop Macros

The range loop macros above can also be used for images, since images are ranges.
But Carma also provides some loop macros that only works for images:

- `FOR_X(x, image)` can be used instead of a normal for-loop
  to let `x` loop from `0` to `image.width - 1`.

- `FOR_Y(y, image)` can be used instead of a normal for-loop
  to let `y` loop from `0` to `image.height - 1`.

FOR_X and FOR_Y can be used independently,
but they can also be used together like in this example:

```c
FOR_Y(y, image) {
    FOR_X(x, image) {
        printf("(x=%i,y=%i) ", x, y);
    }
    printf("\n");
}
```
