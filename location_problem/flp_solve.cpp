#include <iostream>
#include "separation/flp_SeparationProblem.h"
#include "master/flp_MasterProblem.h"
#include "callback_cutting_plane/flp_CuttingPlaneCallback.h"
#include "instance/flp_InstanceFromFile.h"

#define TIME_LIMIT 3600.

using namespace flp;

enum Algorithm { AddScenarioVariables, AddCuts, AddCutsInCallback };

template<enum Algorithm ALG>
bool solve(MasterProblem& t_master, SeparationProblem& t_separation, double t_tolerance) {

    Timer timer;
    timer.start();

    bool has_converged = false;
    while (!has_converged) {

        t_master.set_time_limit(TIME_LIMIT - timer.time_in_seconds());
        t_master.solve();
        if (timer.time_in_seconds() >= TIME_LIMIT) { return false; }
        auto proposition = t_master.get_proposition();
        t_separation.update(proposition);
        t_separation.set_time_limit(TIME_LIMIT - timer.time_in_seconds());
        t_separation.solve();
        if (timer.time_in_seconds() >= TIME_LIMIT) { return false; }
        auto certificate = t_separation.get_certificate();
        if (certificate.objective_value() > t_tolerance) {
            if constexpr(ALG == AddScenarioVariables) {
                t_master.add_scenario_variables(certificate);
            } else {
                t_master.add_benders_cut(certificate);
            }
            t_master.increment_n_added_scenarios();
        } else {
            has_converged = true;
        }

    }

    timer.stop();
    return true;

}

template<>
bool solve<AddCutsInCallback>(MasterProblem& t_master, SeparationProblem& t_separation, double t_tolerance) {

    CuttingPlaneCallback cb(t_master, t_separation);
    t_master.set_callback(cb);
    t_master.set_time_limit(TIME_LIMIT);
    t_master.solve();

    return t_master.timer().time_in_seconds() < TIME_LIMIT;
}

template<enum Algorithm ALG>
void solve_and_report(const flp::Instance& t_instance, double t_tolerance = 1e-8) {

    std::cout << "\n\n" << std::endl;

    SeparationProblem separation(t_instance);
    MasterProblem master(t_instance);

    bool solved = solve<ALG>(master, separation, t_tolerance);

    std::cout << "RESULT,"
              << t_instance.n_sites() << ','
              << t_instance.n_clients() << ','
              << t_instance.gamma() << ','
              << t_instance.deviation() << ','
              << ALG << ','
              << solved << ','
              << master.timer().cumulative_time_in_seconds() << ','
              << separation.timer().cumulative_time_in_seconds() << ','
              << master.objective_value() << ','
              << master.n_added_scenarios()
    << std::endl;
}

int main(int t_argc, const char** t_argv) {

    if (t_argc != 4) { throw std::runtime_error("Expected parameters: <INSTANCE_FILE> <DEVIATION> <ALG=0,1,2>"); }

    const std::string path = t_argv[1];
    const double deviation = std::atof(t_argv[2]);
    const auto algorithm = (Algorithm) std::atoi(t_argv[3]);

    InstanceFromFile instance(path);

    for (unsigned int g = 0, G = instance.n_clients(); g <= G; g += 1) {

        instance.set_robust_parameters(g, deviation);

        switch (algorithm) {
            case AddScenarioVariables: solve_and_report<AddScenarioVariables>(instance);    break;
            case AddCuts: solve_and_report<AddCuts>(instance);                              break;
            case AddCutsInCallback: solve_and_report<AddCutsInCallback>(instance);          break;
        }

    }


    return 0;
}
