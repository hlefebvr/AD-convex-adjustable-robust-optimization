//
// Created by henri on 27.09.23.
//

#include <stdexcept>
#include "ConvexAROSolver.h"

using namespace idol;

void ConvexAROSolver::solve(double t_time_limit, double t_tolerance_for_separation) {

    idol::Timer timer;
    idol::Solution::Primal master_solution;
    idol::Solution::Primal separation_solution;
    unsigned int iteration_count = 0;

    const auto remaining_time = [&]() { return std::max(0., t_time_limit - timer.count()); };

    timer.start();

    initialize();

    do {

        master_solution = solve_master_problem( remaining_time() );

        if (master_solution.status() != Optimal) {
            break;
        }

        update_separation_objective_function(master_solution);

        separation_solution = solve_separation_problem( remaining_time() );

        if (separation_solution.status() != Optimal) {
            break;
        }

        augment_master_problem( separation_solution );

        ++iteration_count;

    } while ( separation_solution.objective_value() <= t_tolerance_for_separation );

    timer.stop();

}
