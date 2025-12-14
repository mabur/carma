cmake_minimum_required(VERSION 3.5)
project(carma_examples)

file(GLOB CARMA_SOURCES "../carma/*.c")

add_executable(tests tests.c ${CARMA_SOURCES})
add_executable(raytracer raytracer.c ${CARMA_SOURCES})
add_executable(aoc22_01 aoc22_01.c ${CARMA_SOURCES})
add_executable(particles particles.c ${CARMA_SOURCES})
add_executable(words words.c ${CARMA_SOURCES})

add_executable(aoc25_day01_part1 advent_of_code_2025/day01_part1.c)
add_executable(aoc25_day01_part2 advent_of_code_2025/day01_part2.c)
add_executable(aoc25_day02_part1 advent_of_code_2025/day02_part1.c)
add_executable(aoc25_day03_part1 advent_of_code_2025/day03_part1.c)
add_executable(aoc25_day04_part1 advent_of_code_2025/day04_part1.c)
add_executable(aoc25_day05_part1 advent_of_code_2025/day05_part1.c)
add_executable(aoc25_day06_part1 advent_of_code_2025/day06_part1.c)

target_compile_features(tests PRIVATE c_std_23)
target_compile_features(raytracer PRIVATE c_std_23)
target_compile_features(aoc22_01 PRIVATE c_std_23)
target_compile_features(particles PRIVATE c_std_23)
target_compile_features(words PRIVATE c_std_23)

target_compile_features(aoc25_day01_part1 PRIVATE c_std_23)
target_compile_features(aoc25_day01_part2 PRIVATE c_std_23)
target_compile_features(aoc25_day02_part1 PRIVATE c_std_23)
target_compile_features(aoc25_day03_part1 PRIVATE c_std_23)
target_compile_features(aoc25_day04_part1 PRIVATE c_std_23)
target_compile_features(aoc25_day05_part1 PRIVATE c_std_23)
target_compile_features(aoc25_day06_part1 PRIVATE c_std_23)

target_include_directories(tests PRIVATE ..)
target_include_directories(raytracer PRIVATE ..)
target_include_directories(aoc22_01 PRIVATE ..)
target_include_directories(particles PRIVATE ..)
target_include_directories(words PRIVATE ..)

target_include_directories(aoc25_day01_part1 PRIVATE ..)
target_include_directories(aoc25_day01_part2 PRIVATE ..)
target_include_directories(aoc25_day02_part1 PRIVATE ..)
target_include_directories(aoc25_day03_part1 PRIVATE ..)
target_include_directories(aoc25_day04_part1 PRIVATE ..)
target_include_directories(aoc25_day05_part1 PRIVATE ..)
target_include_directories(aoc25_day06_part1 PRIVATE ..)

# Add warning flags for GCC
if(CMAKE_C_COMPILER_ID MATCHES "GNU")
    set(WARN_FLAGS
            -Wall           # Enable all standard warnings
            -Wextra         # Enable extra warnings
            -Wpedantic      # Enable strict ISO C and ISO C++ warnings
            -Wshadow        # Warn if a variable declaration shadows one from a parent context
            -Wpointer-arith # Warn about anything that depends on the "size of" a function type or of void
            -Wcast-qual     # Warn whenever a pointer is cast so as to remove a type qualifier from the target type
            -Wstrict-overflow=5 # Detect cases where the compiler optimizes based on the assumption that signed overflow does not occur
            -Wstrict-aliasing=2 # Warn about code which might break the strict aliasing rules
            -Wundef         # Warn if an undefined identifier is evaluated in an #if directive
            -Wcast-align    # Warn whenever a pointer is cast such that the required alignment of the target is increased
    )

    target_compile_options(tests PRIVATE ${WARN_FLAGS})
    target_compile_options(raytracer PRIVATE ${WARN_FLAGS})
    target_compile_options(aoc22_01 PRIVATE ${WARN_FLAGS})
    target_compile_options(particles PRIVATE ${WARN_FLAGS})
    target_compile_options(words PRIVATE ${WARN_FLAGS})
endif()
