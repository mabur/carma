cmake_minimum_required(VERSION 3.5)
project(carma_examples)

add_executable(tests tests.c)
add_executable(raytracer raytracer.c)
add_executable(aoc22_01 aoc22_01.c)

target_compile_features(tests PRIVATE c_std_23)
target_compile_features(raytracer PRIVATE c_std_23)
target_compile_features(aoc22_01 PRIVATE c_std_23)

target_include_directories(tests PRIVATE ..)
target_include_directories(raytracer PRIVATE ..)
target_include_directories(aoc22_01 PRIVATE ..)
