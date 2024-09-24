#include <stdio.h>
#include <carma/carma.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        return EXIT_FAILURE;
    }
    auto file_path = argv[1];
    auto count = 0;
    auto file = fopen(file_path, "r");
    FOR_LINES(line, 255, file) {
        count++;
    }
    fclose(file);
    printf("The file %s contains %d lines\n", file_path, count);
    return EXIT_SUCCESS;
}
