cmake_minimum_required(VERSION 3.5)
project(carma LANGUAGES C)
add_library(carma carma/carma_string.c)
set_target_properties(carma PROPERTIES
        C_STANDARD 23
        C_STANDARD_REQUIRED ON
)
target_include_directories(carma PUBLIC carma)
# add_library(carma INTERFACE "carma/carma.h")
# target_include_directories(carma INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)
