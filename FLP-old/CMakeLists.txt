add_library(flp_instances STATIC instance/flp_Instance.h instance/flp_Instance.cpp instance/flp_InstanceFromFile.cpp instance/flp_InstanceFromFile.h instance/flp_RandomInstance.cpp instance/flp_RandomInstance.h)

add_executable(flp_solve flp_solve.cpp master/flp_FirstStageProposition.h separation/flp_SeparationProblem.cpp separation/flp_SeparationProblem.h master/flp_MasterProblem.cpp master/flp_MasterProblem.h separation/flp_RobustCertificate.cpp separation/flp_RobustCertificate.h master/flp_FirstStageProposition.cpp)

add_executable(flp_generate flp_generate.cpp
        ../FLP/solver/main.cpp)

target_link_libraries(flp_solve PUBLIC flp_instances convex_aro_base)
target_link_libraries(flp_generate PUBLIC flp_instances)
