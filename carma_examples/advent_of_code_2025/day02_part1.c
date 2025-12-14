#include <stdio.h>
#include <carma/carma.h>
#include <carma/carma_string.h>

typedef struct Interval {
    long first;
    long last;
} Interval;

Interval parseInterval(StringView word) {
    auto interval = (Interval){};
    sscanf(word.data, "%ld-%ld", &interval.first, &interval.last);
    return interval;
}

int isComma(int c) {
    return c == ',';
}

bool isInvalidId(long id) {
    char s[32];
    snprintf(s, sizeof(s), "%ld", id);
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
    long sum = 0;
    FOR_EACH_WORD(word, text, isComma) {
        auto interval = parseInterval(word);
        for (auto id = interval.first; id <= interval.last; id++) {
            if (isInvalidId(id)) {
                sum += id;
            }
        }
    }
    printf("%ld\n", sum);
}
