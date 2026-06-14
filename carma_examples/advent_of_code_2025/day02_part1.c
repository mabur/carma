#include <stdio.h>
#include <inttypes.h>
#include <carma/carma.h>
#include <carma/carma_string.h>

#include "carma/carma_parse.h"

typedef struct Interval {
    uint64_t first;
    uint64_t last;
} Interval;

Interval parseInterval(StringView word) {
    auto interval = (Interval){};
    interval.first = PARSE_U64(word);
    DROP_FRONT(word);
    interval.last = PARSE_U64(word);
    return interval;
}

bool isInvalidId(uint64_t id) {
    static StringBuilder s = {};
    CLEAR(s);
    SERIALIZE_INTEGRAL(s, id);
    auto count = s.count;
    if (count % 2 != 0) {
        return false;
    }
    auto half_count = count / 2;
    auto a = (StringView){s.data, half_count};
    auto b = (StringView){s.data + half_count, half_count};
    return ARE_EQUAL(a, b);
}

int main() {
    auto file_path = "day02.txt";
    auto text = read_text_file(file_path);
    APPEND(text, '\0');
    uint64_t sum = 0;
    FOR_EACH_WORD(word, text, ',') {
        auto interval = parseInterval(word);
        for (uint64_t id = interval.first; id <= interval.last; id++) {
            if (isInvalidId(id)) {
                sum += id;
            }
        }
    }
    printf("%" PRIu64 "\n", sum);
}
