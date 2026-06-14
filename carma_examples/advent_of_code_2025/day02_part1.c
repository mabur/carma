#include <stdio.h>
#include <inttypes.h>
#include <carma/carma.h>
#include <carma/carma_string.h>

typedef struct Interval {
    int64_t first;
    int64_t last;
} Interval;

Interval parseInterval(StringView word) {
    auto interval = (Interval){};
    sscanf(word.data, "%" SCNd64 "-%" SCNd64, &interval.first, &interval.last);
    return interval;
}

bool isInvalidId(int64_t id) {
    char s[32];
    snprintf(s, sizeof(s), "%" PRId64, id);
    auto count = strlen(s);
    if (count % 2 != 0) {
        return false;
    }
    auto half_count = count / 2;
    auto a = (StringView){s, half_count};
    auto b = (StringView){s + half_count, half_count};
    return ARE_EQUAL(a, b);
}

int main() {
    auto file_path = "day02.txt";
    auto text = read_text_file(file_path);
    APPEND(text, '\0');
    int64_t sum = 0;
    FOR_EACH_WORD(word, text, ',') {
        auto interval = parseInterval(word);
        for (int64_t id = interval.first; id <= interval.last; id++) {
            if (isInvalidId(id)) {
                sum += id;
            }
        }
    }
    printf("%" PRId64 "\n", sum);
}
