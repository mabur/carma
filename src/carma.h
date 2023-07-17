#define let __auto_type // Requires GNUC. C23 also has auto.

#define INIT_RANGE(range, count) do { \
    (range).first = malloc((count) * sizeof(*(range).first)); \
    (range).last = (range).first + (count); \
} while (0);

#define FREE_RANGE(range) do { \
    free((range).first); \
    (range).first = NULL; \
    (range).last = NULL; \
} while (0);

#define FOR_RANGE(it, range) \
    for (__auto_type it = (range).first; it != (range).last; ++it)
