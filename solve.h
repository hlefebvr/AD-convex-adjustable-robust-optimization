//
// Created by henri on 28/07/22.
//

#ifndef CONVEX_ARO_SOLVE_H
#define CONVEX_ARO_SOLVE_H

#include <iostream>
#include <iomanip>

static constexpr double TIME_LIMIT = 3600.;
static constexpr bool VERBOSE = true;

enum Algorithm {
    ColumnAndConstraintGeneration,
    GeneralizedBendersDecomposition
};

enum ProblemType {
    Master,
    Separation
};

std::ostream& operator<<(std::ostream& t_os, Algorithm t_alg) {
    switch (t_alg) {
        case ColumnAndConstraintGeneration: return t_os << "COLUMN_AND_CONSTRAINT_GENERATION";
        case GeneralizedBendersDecomposition: return t_os << "GENERALIZED_BENDERS_DECOMPOSITION";
    }
    return t_os << "UNKNOWN";
}

std::ostream& operator<<(std::ostream& t_os, ProblemType t_type) {
    switch (t_type) {
        case Master: return t_os << "MASTER";
        case Separation: return t_os << "SEPARATION";
    }
    return t_os << "UNKNOWN";
}

void log(ProblemType t_type, double t_obj, double t_time, double t_total_time, unsigned int t_n_added_scenarios) {
    std::cout
        << "[INFO]" << '\t'
        << std::setw(10) << t_type << '\t'
        << std::setw(10) << t_obj << '\t'
        << std::setw(10) << t_time << '\t'
        << std::setw(10) << t_total_time << '\t'
        << std::setw(10) << t_n_added_scenarios << '\t'
        << std::endl;
}

template<Algorithm ALG, class MasterProblem, class SeparationProblem>
bool solve(MasterProblem& t_master, SeparationProblem& t_separation, double t_tolerance) {

    auto total_time = [&]() { return t_master.timer().cumulative_time_in_seconds() + t_separation.timer().cumulative_time_in_seconds(); };

    bool has_converged = false;
    while (!has_converged) {

        t_master.set_time_limit(TIME_LIMIT - total_time());
        t_master.solve();
        auto proposition = t_master.get_proposition();

        if constexpr (VERBOSE) {
            log(Master, proposition.objective_value(), t_master.timer().time_in_seconds(), total_time(), t_master.n_added_scenarios());
        }

        if (total_time() >= TIME_LIMIT) { return false; }

        t_separation.update(proposition);
        t_separation.set_time_limit(TIME_LIMIT - total_time());
        t_separation.solve();
        auto certificate = t_separation.get_certificate();

        if constexpr (VERBOSE) {
            log(Separation, certificate.objective_value(), t_separation.timer().time_in_seconds(), total_time(), t_master.n_added_scenarios());
        }

        if (total_time() >= TIME_LIMIT) { return false; }

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
void solve_and_report(std::ostream& t_os, MasterProblem& master, SeparationProblem& separation, Algorithm t_algorithm, double t_tolerance = 1e-2) {

    bool solved;
    switch (t_algorithm) {
        case ColumnAndConstraintGeneration: solved = solve<ColumnAndConstraintGeneration>(master, separation, t_tolerance); break;
        case GeneralizedBendersDecomposition: solved = solve<GeneralizedBendersDecomposition>(master, separation, t_tolerance); break;
        default: throw std::runtime_error("Unknown algorithm");
    }

    t_os << "RESULT,"
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
