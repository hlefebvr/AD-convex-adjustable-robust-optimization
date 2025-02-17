cmake_minimum_required(VERSION 3.22)
project(ConvexAdjustalbeRobustOptimization)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

#set(USE_GUROBI ON)
set(USE_MOSEK ON)
set(USE_EIGEN ON)

FetchContent_Declare(
        idol
        GIT_REPOSITORY https://github.com/hlefebvr/idol.git
        GIT_TAG        v0.8.2-alpha-fixed # origin/main
)

FetchContent_MakeAvailable(idol)

add_library(convex_aro ConvexAROSolver.h ConvexAROSolver.cpp)
target_link_libraries(convex_aro PUBLIC idol)

add_subdirectory(FLP-with-congestion)
add_subdirectory(FLP-with-convex-production-costs)
add_subdirectory(RAP)