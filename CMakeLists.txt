cmake_minimum_required(VERSION 3.22)
project(convex_aro)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

add_executable(convex_aro main.cpp location_problem/instance/flp_Instance.h location_problem/instance/flp_Instance.cpp location_problem/instance/flp_InstanceFromFile.cpp location_problem/instance/flp_InstanceFromFile.h location_problem/instance/flp_RandomInstance.cpp location_problem/instance/flp_RandomInstance.h location_problem/master/flp_FirstStageProposition.h location_problem/separation/flp_SeparationProblem.cpp location_problem/separation/flp_SeparationProblem.h location_problem/master/flp_MasterProblem.cpp location_problem/master/flp_MasterProblem.h Model.cpp Model.h location_problem/cutting_plane/flp_CuttingPlaneCallback.cpp location_problem/cutting_plane/flp_CuttingPlaneCallback.h location_problem/cutting_plane/flp_Callback.h location_problem/cutting_plane/flp_Callback.cpp location_problem/separation/flp_RobustCertificate.cpp location_problem/separation/flp_RobustCertificate.h Timer.cpp Timer.h)

find_package(GUROBI REQUIRED)
target_link_libraries(convex_aro PUBLIC gurobi)