if (NOT GUROBI_DIR)
    set(GUROBI_DIR "/opt/gurobi951/linux64")
endif()

find_path(
        GUROBI_INCLUDE_DIRS
        NAMES gurobi_c.h
        HINTS ${GUROBI_DIR} $ENV{GUROBI_HOME}
        PATH_SUFFIXES include)

find_library(
        GUROBI_LIBRARY
        NAMES gurobi gurobi81 gurobi90 gurobi95
        HINTS ${GUROBI_DIR} $ENV{GUROBI_HOME}
        PATH_SUFFIXES lib)

find_library(
        GUROBI_CPP_LIBRARY
        NAMES gurobi_c++
        HINTS ${GUROBI_DIR} $ENV{GUROBI_HOME}
        PATH_SUFFIXES lib
)

file(GLOB GUROBI_CXX_SRC $ENV{GUROBI_HOME}/src/cpp/*.cpp)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GUROBI DEFAULT_MSG GUROBI_LIBRARY GUROBI_INCLUDE_DIRS GUROBI_CPP_LIBRARY)

if (GUROBI_FOUND)
    add_library(gurobi STATIC EXCLUDE_FROM_ALL ${GUROBI_CXX_SRC})
    target_include_directories(gurobi PUBLIC ${GUROBI_INCLUDE_DIRS})
    target_link_libraries(gurobi PUBLIC ${GUROBI_LIBRARY})
else()
    message(FATAL_ERROR "Could not find GUROBI")
endif()
