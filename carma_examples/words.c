#include <stdio.h>
#include <carma/carma.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        return EXIT_FAILURE;
    }
    auto file_path = argv[1];
    auto count = 0;
    READ_LINES(line, 255, file_path) {
        count++;
    }
    printf("The file %s contains %d lines\n", file_path, count);
    return EXIT_SUCCESS;
}
