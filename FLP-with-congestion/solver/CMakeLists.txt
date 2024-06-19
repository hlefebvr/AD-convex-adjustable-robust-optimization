add_executable(FLP_cong_solve
        main.cpp
        ../../ConvexAROSolver.h
        CCG.cpp
        CCG.h
        GBD.cpp
        GBD.h
        Solver.cpp
        Solver.h
        Nominal.cpp
        Nominal.h)
target_link_libraries(FLP_cong_solve PUBLIC convex_aro)
