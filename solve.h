//
// Created by henri on 28/07/22.
//

#ifndef CONVEX_ARO_SOLVE_H
#define CONVEX_ARO_SOLVE_H

#include <iostream>
#include <iomanip>

#define TIME_LIMIT 3600.

enum Algorithm {
    GBD,
    CCG,
};

enum ProblemType {
    Master,
    Separation,
};

std::ostream& operator<<(std::ostream& t_os, Algorithm t_alg) {
    switch (t_alg) {
        case CCG: return t_os << "CCG";
        case GBD: return t_os << "GBD";
    }
    return t_os << "Unknown";
}

std::ostream& operator<<(std::ostream& t_os, ProblemType t_type) {
    switch (t_type) {
        case Master: return t_os << "Master";
        case Separation: return t_os << "Separation";
    }
    return t_os << "Unknown";
}

void log(Algorithm t_algorithm, ProblemType t_type, double t_obj, double t_lb, double t_ub, double t_gap, double t_time, double t_total_time, unsigned int t_n_added_scenarios) {
    std::cout
        << "[info]" << '\t'
        << t_algorithm << '\t'
        << std::setw(10) << t_type << '\t'
        << std::setw(10) << t_obj << '\t'
        << std::setw(10) << t_lb << '\t'
        << std::setw(10) << t_ub << '\t'
        << std::setw(10) << t_gap * 100 << '\t'
        << std::setw(10) << t_time << '\t'
        << std::setw(10) << t_total_time << '\t'
        << std::setw(10) << t_n_added_scenarios << '\t'
        << std::endl;
}

template<Algorithm Algorithm,
        class MasterProblem,
        class SeparationProblem,
        bool Verbose = false>
bool solve(MasterProblem& t_master, SeparationProblem& t_separation, double t_tolerance) {

    auto total_time = [&]() { return t_master.timer().cumulative_time_in_seconds() + t_separation.timer().cumulative_time_in_seconds(); };

    auto gap = [](double t_lb, double t_ub) {
        if (t_lb < -1e100 || t_ub > 1e100) {
            return std::numeric_limits<double>::max();
        }
        return std::abs(t_lb - t_ub) / (std::abs(t_ub) + 1e-10);
    };

    double LB = std::numeric_limits<double>::lowest();
    double UB = std::numeric_limits<double>::max();

    while (true) {

        t_master.set_time_limit(TIME_LIMIT - total_time());
        t_master.solve();
        auto proposition = t_master.get_proposition();

        LB = std::max(LB, proposition.objective_value());

        if constexpr (Verbose) {
            log(Algorithm, Master, proposition.objective_value(), LB, UB, gap(LB, UB), t_master.timer().time_in_seconds(), total_time(), t_master.n_added_scenarios());
        }

        if (total_time() >= TIME_LIMIT) { return false; }

        //if (gap(LB, UB) <= t_tolerance) { return true; }

        t_separation.update(proposition);
        t_separation.set_time_limit(TIME_LIMIT - total_time());
        t_separation.solve();
        auto certificate = t_separation.get_certificate();

        //UB = std::min(UB, proposition.objective_value() + certificate.objective_value());

        if constexpr (Verbose) {
            log(Algorithm, Separation, certificate.objective_value(), LB, UB, gap(LB, UB), t_separation.timer().time_in_seconds(), total_time(), t_master.n_added_scenarios());
        }

        if (total_time() >= TIME_LIMIT) { return false; }

        //if (gap(LB, UB) <= t_tolerance) { return true; }

        if (certificate.objective_value() <= t_tolerance) {
            return true;
        }

        if constexpr(Algorithm == CCG) {
            t_master.add_scenario_variables(certificate);
        } else {
            t_master.add_benders_cut(certificate);
        }

        t_master.increment_n_added_scenarios();

    }

}

template<class MasterProblem, class SeparationProblem>
void solve_and_report(std::ostream& t_os, MasterProblem& master, SeparationProblem& separation, Algorithm t_algorithm, double t_tolerance = 1e-3) {

    bool solved;
    switch (t_algorithm) {
        case CCG: solved = solve<CCG>(master, separation, t_tolerance); break;
        case GBD: solved = solve<GBD>(master, separation, t_tolerance); break;
        default: throw std::runtime_error("Unknown algorithm");
    }

    t_os
      << t_algorithm << ','
      << (solved ? "Optimal" : "TimeLimit") << ','
      << master.timer().cumulative_time_in_seconds() << ','
      << separation.timer().cumulative_time_in_seconds() << ','
      << (master.timer().cumulative_time_in_seconds() + separation.timer().cumulative_time_in_seconds()) << ','
      << master.objective_value() << ','
      << master.n_added_scenarios()
      << std::endl;
}


#endif //CONVEX_ARO_SOLVE_H
