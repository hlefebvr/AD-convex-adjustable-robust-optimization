add_library(rap_instances STATIC instance/rap_Instance.h instance/rap_Instance.cpp instance/rap_RandomInstance.cpp instance/rap_RandomInstance.h instance/rap_InstanceFromFile.cpp instance/rap_InstanceFromFile.h)

add_executable(rap_solve rap_solve.cpp)

add_executable(rap_generate rap_generate.cpp)

target_link_libraries(rap_solve PUBLIC rap_instances convex_aro_base)
target_link_libraries(rap_generate PUBLIC rap_instances)
