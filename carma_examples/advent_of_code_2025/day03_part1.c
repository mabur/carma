#include <stdio.h>
#include <carma/carma.h>
#include <carma/carma_string.h>

int maxJoltage(const char* max_it, const char* max_before_it, const char* max_after_it, const char* end) {
    char s[3];
    if (max_after_it == end) {
        s[0] = *max_before_it;
        s[1] = *max_it;
    }
    else {
        s[0] = *max_it;
        s[1] = *max_after_it;
    }
    s[3] = '\0';
    auto value = -1;
    sscanf(s, "%d", &value);
    return value;
}

int main() {
    auto file_path = "day03.txt";
    auto capacity = 100000;
    auto sum = 0;
    READ_LINES(line, capacity, file_path) {
        auto battery = (StringView){line, strlen(line) - 1};
        FOR_MAX(max_it, battery) {}
        auto before = (StringView){battery.data, max_it - battery.data};
        FOR_MAX(max_before_it, before) {}
        auto after = (StringView){max_it, battery.data + battery.count - max_it};
        DROP_FRONT(after);
        FOR_MAX(max_after_it, after) {}
        auto max_joltage = maxJoltage(max_it, max_before_it, max_after_it, END_POINTER(battery));
        sum += max_joltage;
    }
    printf("%d\n", sum);
}
