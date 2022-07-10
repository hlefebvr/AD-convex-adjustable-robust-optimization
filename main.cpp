#include <iostream>
#include "location_problem/instance/flp_RandomInstance.h"
#include "location_problem/separation/flp_SeparationProblem.h"
#include "location_problem/master/flp_MasterProblem.h"
#include "location_problem/callback_cutting_plane/flp_CuttingPlaneCallback.h"
#include "location_problem/instance/flp_InstanceFromFile.h"

using namespace flp;

enum Algorithm { AddScenarioVariables, AddCuts, AddCutsInCallback };

template<enum Algorithm ALG>
void solve(MasterProblem& t_master, SeparationProblem& t_separation, double t_tolerance) {

    bool has_converged = false;
    while (!has_converged) {

        t_master.solve();
        auto proposition = t_master.get_proposition();
        t_separation.update(proposition);
        t_separation.solve();
        auto certificate = t_separation.get_certificate();
        if (certificate.objective_value() > t_tolerance) {
            if constexpr(ALG == AddScenarioVariables) {
                t_master.add_scenario_variables(certificate);
            } else {
                t_master.add_benders_cut(certificate);
            }
        } else {
            has_converged = true;
        }

    }

}

template<>
void solve<AddCutsInCallback>(MasterProblem& t_master, SeparationProblem& t_separation, double t_tolerance) {

    CuttingPlaneCallback cb(t_separation);
    t_master.set_callback(cb);
    t_master.solve();

}

template<enum Algorithm ALG>
void solve_and_report(const flp::Instance& t_instance, double t_tolerance = 1e-8) {

    std::cout << "\n\n" << std::endl;

    SeparationProblem separation(t_instance);
    MasterProblem master(t_instance);

    solve<ALG>(master, separation, t_tolerance);

    std::cout << "RESULT,"
              << t_instance.n_sites() << ','
              << t_instance.n_clients() << ','
              << t_instance.gamma() << ','
              << t_instance.deviation() << ','
              << ALG << ','
              << master.timer().cumulative_time_in_seconds() << ','
              << separation.timer().cumulative_time_in_seconds() << ','
              << master.objective_value() << ','
    << std::endl;
}

int main() {

    using namespace flp;

    const double deviation = .15;

    const std::vector<std::pair<unsigned int, unsigned int>> configs = {
            { 4, 8 },
            { 5, 20 },
            { 10, 20 },
            { 10, 30 },
            { 15, 30 },
            { 15, 40 },
            { 20, 40 },
            { 20, 50 },
            { 25, 50 },
            { 25, 60 },
            { 30, 60 },
            { 30, 70 }
    };

    for (const auto& [n_sites, n_clients] : configs) {

        for (unsigned int k = 0 ; k < 5 ; k += 1) {

            RandomInstance instance(n_sites, n_clients, 1.4);
            //InstanceFromFile instance("instance.txt");

            for (unsigned int Gamma = 0; Gamma <= n_clients; Gamma += 1) {

                instance.set_robust_parameters(Gamma, deviation);

                try {
                    solve_and_report<AddCuts>(instance);
                    solve_and_report<AddCutsInCallback>(instance);
                    solve_and_report<AddScenarioVariables>(instance);
                } catch (const GRBException& err) {
                    std::cout << err.getMessage() << std::endl;
                    __throw_exception_again;
                }
            }

            throw 10;

        }

    }

    return 0;
}
