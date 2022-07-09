#include <iostream>
#include <fstream>
#include <chrono>
#include "location_problem/flp_Instance.h"
#include "location_problem/flp_RandomInstance.h"
#include "location_problem/flp_SeparationProblem.h"
#include "location_problem/flp_MasterProblem.h"
#include "location_problem/flp_CuttingPlaneCallback.h"
#include "location_problem/flp_InstanceFromFile.h"

template<class T> void solve(const flp::Instance& t_instance) {

    using namespace flp;

    SeparationProblem separation(t_instance);
    T cb(t_instance, separation);

    MasterProblem master(t_instance);
    master.set_callback(cb);
    master.solve();

    std::cout << "RESULT," << t_instance.n_sites() << ','
              << t_instance.n_clients() << ','
              << t_instance.gamma() << ','
              << t_instance.deviation() << ','
              << master.timer().cumulative_time_in_seconds() << ','
              << separation.timer().cumulative_time_in_seconds() << ','
              << master.objective_value() << ','
              << cb.n_solved_separation() << ','
              << cb.n_generated_scenarios() << ','
              << std::endl;

}

int main() {

    using namespace flp;

    const double deviation = .15;

    // InstanceFromFile instance("instance.txt");

    const std::vector<std::pair<unsigned int, unsigned int>> configs = {
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

            RandomInstance instance(15, 30, 1.4);

            for (unsigned int Gamma = 0; Gamma < n_clients; Gamma += 1) {

                instance.set_robust_parameters(Gamma, deviation);

                solve<CuttingPlaneCallback>(instance);

            }

        }

    }

    return 0;
}
