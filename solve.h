//
// Created by henri on 28/07/22.
//

#ifndef CONVEX_ARO_SOLVE_H
#define CONVEX_ARO_SOLVE_H

#include <iostream>

#define TIME_LIMIT 3600.

enum Algorithm {
    ColumnAndConstraintGeneration,
    GeneralizedBendersDecomposition
};

std::ostream& operator<<(std::ostream& t_os, Algorithm t_alg) {
    switch (t_alg) {
        case ColumnAndConstraintGeneration: return t_os << "COLUMN_AND_CONSTRAINT_GENERATION";
        case GeneralizedBendersDecomposition: return t_os << "GENERALIZED_BENDERS_DECOMPOSITION";
    }
    return t_os << "UNKNOWN";
}

template<Algorithm ALG, class MasterProblem, class SeparationProblem>
bool solve(MasterProblem& t_master, SeparationProblem& t_separation, double t_tolerance) {

    auto total_time = [&]() { return t_master.timer().cumulative_time_in_seconds() + t_separation.timer().cumulative_time_in_seconds(); };

    bool has_converged = false;
    while (!has_converged) {

        t_master.set_time_limit(TIME_LIMIT - total_time());
        t_master.solve();

        if (total_time() >= TIME_LIMIT) { return false; }

        auto proposition = t_master.get_proposition();
        t_separation.update(proposition);
        t_separation.set_time_limit(TIME_LIMIT - total_time());
        t_separation.solve();

        if (total_time() >= TIME_LIMIT) { return false; }

        auto certificate = t_separation.get_certificate();
        if (certificate.objective_value() > t_tolerance) {

            if constexpr(ALG == ColumnAndConstraintGeneration) {
                t_master.add_scenario_variables(certificate);
            } else {
                t_master.add_benders_cut(certificate);
            }

            t_master.increment_n_added_scenarios();

        } else {
            has_converged = true;
        }

    }

    return true;

}


template<class MasterProblem, class SeparationProblem>
void solve_and_report(MasterProblem& master, SeparationProblem& separation, Algorithm t_algorithm, double t_tolerance = 1e-3) {

    bool solved;
    switch (t_algorithm) {
        case ColumnAndConstraintGeneration: solved = solve<ColumnAndConstraintGeneration>(master, separation, t_tolerance); break;
        case GeneralizedBendersDecomposition: solved = solve<GeneralizedBendersDecomposition>(master, separation, t_tolerance); break;
        default: throw std::runtime_error("Unknown algorithm");
    }

    std::cout << "RESULT,"
              << t_algorithm << ','
              << (solved ? "OPTIMAL" : "TIME_LIMIT") << ','
              << master.timer().cumulative_time_in_seconds() << ','
              << separation.timer().cumulative_time_in_seconds() << ','
              << (master.timer().cumulative_time_in_seconds() + separation.timer().cumulative_time_in_seconds()) << ','
              << master.objective_value() << ','
              << master.n_added_scenarios()
              << std::endl;
}


#endif //CONVEX_ARO_SOLVE_H
