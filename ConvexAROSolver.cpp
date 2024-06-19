//
// Created by henri on 27.09.23.
//

#include <stdexcept>
#include "ConvexAROSolver.h"

using namespace idol;

AbstractSolver::Report ConvexAROSolver::solve(double t_time_limit,
                                              double t_tolerance_for_separation,
                                              bool t_heuristic_mode) {

    idol::Timer timer, master_timer, separation_timer;
    idol::Solution::Primal master_solution;
    idol::Solution::Primal separation_solution;
    unsigned int iteration_count = 0;
    m_heuristic_mode = t_heuristic_mode;

    const auto remaining_time = [&]() { return std::max(0., t_time_limit - timer.count()); };

    const auto log_iteration = [&]() {
        std::cout
            << "<Iter=" << iteration_count << "> "
            << "<TotalTime=" << timer.count() << "> "
            << "<IterMasterTime=" << master_timer.count() << "> "
            << "<IterSepTime=" << separation_timer.count() << "> "
            << "<BestBound=" << master_solution.objective_value() << "> "
            << "<Violation=" << separation_solution.objective_value() << "> "
            << "<IterStatus=" << master_solution.status() << "," << separation_solution.status() << "> "
            << std::endl;
    };

    timer.start();

    initialize();

    do {

        master_timer.start();
        master_solution = solve_master_problem( remaining_time() );
        master_timer.stop();

        if (master_solution.status() != Optimal) {
            break;
        }

        update_separation_objective_function(master_solution);

        separation_timer.start();
        separation_solution = solve_separation_problem( remaining_time() );
        separation_timer.stop();

        if (separation_solution.status() != Optimal && !(m_heuristic_mode && separation_solution.status() == Feasible)) {
            break;
        }

        augment_master_problem( separation_solution );

        ++iteration_count;

        log_iteration();

        if (separation_solution.objective_value() <= t_tolerance_for_separation) {
            if (m_heuristic_mode) {
                m_heuristic_mode = false;
                continue;
            }
            break;
        }

    } while ( true );

    timer.stop();

    return {
        master_solution.has_objective_value() ? master_solution.objective_value() : Inf,
        timer.count(),
        master_timer.cumulative_count(),
        separation_timer.cumulative_count(),
        iteration_count,
        (master_solution.status() != Optimal && master_solution.reason() != TimeLimit)
        || (separation_solution.status() != Optimal && separation_solution.reason() != TimeLimit)
    };

}

AbstractSolver::Report::Report(double t_best_bound,
                                   double t_total_time,
                                   double t_master_time,
                                   double t_separation_time,
                                   unsigned int t_iteration_count,
                                   bool t_fail_flag)
                                   : best_bound(t_best_bound),
                                     total_time(t_total_time),
                                     master_time(t_master_time),
                                     separation_time(t_separation_time),
                                     iteration_count(t_iteration_count),
                                     fail_flag(t_fail_flag) {

}

void EarlyStopCallback::Strategy::operator()(idol::CallbackEvent t_event) {

    if (t_event != IncumbentSolution) {
        return;
    }

    if (!m_parent->heuristic_mode()) {
        return;
    }

    if (time().count() < 10 || is_inf(best_obj())) {
        return;
    }

    if (best_obj() < 1e-4) {
        return;
    }

    terminate();

}

EarlyStopCallback::Strategy::Strategy(const ConvexAROSolver &t_parent) : m_parent(&t_parent) {

}
