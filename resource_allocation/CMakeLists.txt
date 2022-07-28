add_library(rap_instances STATIC instance/rap_Instance.h instance/rap_Instance.cpp instance/rap_RandomInstance.cpp instance/rap_RandomInstance.h instance/rap_InstanceFromFile.cpp instance/rap_InstanceFromFile.h)

add_executable(rap_solve rap_solve.cpp master/rap_MasterProblem.cpp master/rap_MasterProblem.h master/rap_FirstStageProposition.cpp master/rap_FirstStageProposition.h separation/rap_RobustCertificate.cpp separation/rap_RobustCertificate.h)

add_executable(rap_generate rap_generate.cpp)

target_link_libraries(rap_solve PUBLIC rap_instances convex_aro_base)
target_link_libraries(rap_generate PUBLIC rap_instances)
