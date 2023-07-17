cmake_minimum_required(VERSION 3.5)
project(carma)
add_executable(raytracer src/raytracer.c)
target_compile_features(raytracer PRIVATE c_std_99)
