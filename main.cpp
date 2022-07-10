#include <iostream>
#include "location_problem/instance/flp_RandomInstance.h"
#include "location_problem/separation/flp_SeparationProblem.h"
#include "location_problem/master/flp_MasterProblem.h"
#include "location_problem/cutting_plane/flp_CuttingPlaneCallback.h"

void solve_with_branch_and_cut(const flp::Instance& t_instance) {

    using namespace flp;

    SeparationProblem separation(t_instance);
    CuttingPlaneCallback cb(t_instance, separation);

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

            RandomInstance instance(n_sites, n_clients, 1.4);

            for (unsigned int Gamma = 0; Gamma <= n_clients; Gamma += 1) {

                instance.set_robust_parameters(Gamma, deviation);

                solve_with_branch_and_cut(instance);

            }

        }

    }

    return 0;
}
