cmake_minimum_required(VERSION 3.22)
project(convex_aro)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(GUROBI REQUIRED)

add_library(convex_aro_base STATIC Model.h Model.cpp Timer.h Timer.cpp cout_utils.h cin_utils.h)
target_link_libraries(convex_aro_base PUBLIC gurobi)

add_subdirectory(location_problem)
add_subdirectory(resource_allocation)