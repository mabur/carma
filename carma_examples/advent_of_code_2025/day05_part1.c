#include <stdio.h>
#include <inttypes.h>
#include <carma/carma.h>
#include <carma/carma_string.h>
#include <carma/carma_parse.h>

typedef struct Interval {
    uint64_t first;
    uint64_t last;
} Interval;

typedef struct Intervals {
    Interval* data;
    size_t count;
    size_t capacity;
} Intervals;

typedef struct Ids {
    uint64_t* data;
    size_t count;
    size_t capacity;
} Ids;

bool isInsideInterval(uint64_t id, Interval interval) {
    return interval.first <= id && id <= interval.last;
}

bool isInsideAnyInterval(uint64_t id, Intervals intervals) {
    FOR_EACH(interval, intervals) {
        if (isInsideInterval(id,*interval)) {
            return true;
        }
    }
    return false;
}

int countFreshIds(Ids ids, Intervals intervals) {
    auto count = 0;
    FOR_EACH(id, ids) {
        if (isInsideAnyInterval(*id, intervals)) {
            count += 1;
        }
    }
    return count;
}

int main() {
    auto file_path = "day05.txt";
    auto capacity = 100000;
    auto read_interval = true;
    auto intervals = (Intervals){};
    auto ids = (Ids){};
    READ_LINES(line, capacity, file_path) {
        if (strlen(line) == 1) {
            read_interval = false;
            continue;
        }
        auto word = STRING_VIEW(line);
        if (read_interval) {
            auto interval = (Interval){};
            interval.first = PARSE_U64(word);
            DROP_FRONT(word);
            interval.last = PARSE_U64(word);
            APPEND(intervals, interval);
        }
        else {
            auto id = PARSE_U64(word);
            APPEND(ids, id);
        }
    }
    auto count = countFreshIds(ids, intervals);
    printf("Count: %d\n", count);
}
