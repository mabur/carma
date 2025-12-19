#include <stdio.h>
#include <carma/carma.h>
#include <carma/carma_string.h>

#define MIN2(a, b) ((a) < (b) ? (a) : (b))
#define MAX2(a, b) ((a) > (b) ? (a) : (b))

typedef struct Map {
    char* data;
    size_t count;
    size_t width;
    size_t height;
} Map;

Map readMap(const char* file_path) {
    auto capacity = 100000;
    size_t width = 0;
    size_t height = 0;
    READ_LINES(line, capacity, file_path) {
        width = MAX2(width, strlen(line) - 1);
        height += 1;
    }
    auto map = (Map){};
    INIT_2D_ARRAY(map, width, height);
    auto y = 0;
    READ_LINES(line, capacity, file_path) {
        auto row = (StringView){line, strlen(line) - 1};
        FOR_INDEX(x, row) {
            AT_XY(map, x,y) = row.data[x];
        }
        y += 1;
    }
    return map;
}

#define AT_XY_OR(image, x, y, default_value) \
    IS_INSIDE_ARRAY2D((image), (x), (y)) ? AT_XY((image), (x), (y)) : (default_value);

int main() {
    auto file_path = "day04.txt";
    auto map = readMap(file_path);
    auto count = 0;
    auto EMPTY = '.';
    FOR_Y(y, map) {
        FOR_X(x, map) {
            if (AT_XY(map, x, y) == EMPTY) {
                continue;
            }
            auto N  = AT_XY_OR(map, x,     y - 1, EMPTY);
            auto NE = AT_XY_OR(map, x + 1, y - 1, EMPTY);
            auto E  = AT_XY_OR(map, x + 1, y,     EMPTY);
            auto SE = AT_XY_OR(map, x + 1, y + 1, EMPTY);
            auto S  = AT_XY_OR(map, x,     y + 1, EMPTY);
            auto SW = AT_XY_OR(map, x - 1, y + 1, EMPTY);
            auto W  = AT_XY_OR(map, x - 1, y,     EMPTY);
            auto NW = AT_XY_OR(map, x - 1, y - 1, EMPTY);
            auto occupied_neighbour_count = (
                (N  != EMPTY) +
                (NE != EMPTY) +
                (E  != EMPTY) +
                (SE != EMPTY) +
                (S  != EMPTY) +
                (SW != EMPTY) +
                (W  != EMPTY) +
                (NW != EMPTY)
            );
            if (occupied_neighbour_count < 4) {
                count += 1;
            }
        }
    }
    printf("%d\n", count);
}
