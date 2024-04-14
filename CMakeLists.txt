cmake_minimum_required(VERSION 3.5)
project(carma)
add_library(carma INTERFACE "carma/carma.h")
#target_include_directories(carma INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)
