#include <stdio.h>
#include <carma/carma.h>
#include <carma/carma_string.h>

typedef struct Interval {
    long first;
    long last;
} Interval;

typedef struct Intervals {
    Interval* data;
    size_t count;
    size_t capacity;
} Intervals;

typedef struct Ids {
    long* data;
    size_t count;
    size_t capacity;
} Ids;

bool isInsideInterval(long id, Interval interval) {
    return interval.first <= id && id <= interval.last;
}

bool isInsideAnyInterval(long id, Intervals intervals) {
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
        if (read_interval) {
            auto interval = (Interval){};
            sscanf(line, "%ld-%ld", &interval.first, &interval.last);
            APPEND(intervals, interval);
        }
        else {
            long id = -1;
            sscanf(line, "%ld", &id);
            APPEND(ids, id);
        }
    }
    auto count = countFreshIds(ids, intervals);
    printf("Count: %d\n", count);
}
