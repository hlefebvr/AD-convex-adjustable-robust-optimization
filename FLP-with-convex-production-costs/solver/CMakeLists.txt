add_executable(FLP_conv_solve
        main.cpp
        Solver.cpp
        Solver.h
        Nominal.cpp
        Nominal.h
        CCG.cpp
        CCG.h
        GBD.cpp
        GBD.h
)
target_link_libraries(FLP_conv_solve PUBLIC convex_aro)
