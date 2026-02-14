## Json Parsing

Let's say that you want to parse this json string:

```
{
    "width": 640,
    "height:" 480,
    "points_x": [300.0, 100.0, 500.0],
    "points_y": [400.0, 200.0, 100.0]
}
```

Carma implements macros for doing this in `carma_json_parse.h`.
You use them like this:

```clike
    StringView json = ...;
    int width = PARSE_JSON_KEY_INT(json, "width");
    int height = PARSE_JSON_KEY_INT(json, "height");
    StringView points_x = PARSE_JSON_KEY_ARRAY(json, "points_x");
    StringView points_y = PARSE_JSON_KEY_ARRAY(json, "points_y");
    FOR_EACH_JSON_ARRAY_ITEM(item, points_x) {
        double x = PARSE_DOUBLE(item);
    }
    FOR_EACH_JSON_ARRAY_ITEM(item, points_x) {
        double y = PARSE_DOUBLE(item);
    }
```
