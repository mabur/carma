#include <stdio.h>
#include <carma/carma.h>
#include <carma/carma_string.h>

int parseRoation(const char* line) {
    auto magnitude = strtol(line + 1, NULL, 10);
    return line[0] == 'R' ? magnitude : -magnitude;
}

#define WRAP(n, count) (((n) % (count) + (count)) % (count))

int main() {
    auto dial = 50;
    auto zero_count = 0;
    auto file_path = "day01.txt";
    auto capacity = 100000;
    READ_LINES(line, capacity, file_path) {
        auto rotation = parseRoation(line);
        dial += rotation;
        dial = WRAP(dial, 100);
        if (dial == 0) zero_count++;
    }
    printf("Zero count: %d\n", zero_count);
}
