## Json Serialization

Let's say that you want to write this json string:

```
{
    "visible": true,
    "width": 640,
    "height:" 480,
    "points_x": [300.0, 100.0, 500.0],
    "points_y": [400.0, 200.0, 100.0]
}
```

Carma implements macros for doing this in `carma_json_serialize.h`.
You use them like this:

```clike
auto j = (JsonBuilder){};
ADD_JSON_OBJECT(j) {
    ADD_JSON_KEY(j, "visible");
    ADD_JSON_BOOL(j, true);
    ADD_JSON_KEY(j, "width");
    ADD_JSON_INT(j, 640);
    ADD_JSON_KEY(j, "height");
    ADD_JSON_INT(j, 480);
    ADD_JSON_KEY(j, "points_x");
    ADD_JSON_ARRAY(j) {
        ADD_JSON_DOUBLE(j, 300.0);
        ADD_JSON_DOUBLE(j, 100.0);
        ADD_JSON_DOUBLE(j, 500.0);
    }
    ADD_JSON_KEY(j, "points_y");
    ADD_JSON_ARRAY(j) {
        ADD_JSON_DOUBLE(j, 400.0);
        ADD_JSON_DOUBLE(j, 200.0);
        ADD_JSON_DOUBLE(j, 100.0);
    }
}
```

This writes a json string to the member `string` of the `JsonBuilder` struct.
The member `string` has the type `StringBuilder`.
