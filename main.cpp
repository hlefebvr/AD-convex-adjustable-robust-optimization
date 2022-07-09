#include <iostream>
#include <fstream>
#include <chrono>
#include "location_problem/flp_Instance.h"
#include "location_problem/flp_RandomInstance.h"
#include "location_problem/flp_SeparationProblem.h"
#include "location_problem/flp_MasterProblem.h"
#include "location_problem/flp_CuttingPlaneCallback.h"
#include "location_problem/flp_InstanceFromFile.h"

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

                SeparationProblem separation(instance);
                CuttingPlaneCallback cb(instance, separation);

                MasterProblem master(instance);
                master.set_callback(cb);
                master.solve();

                std::cout << "RESULT," << n_sites << ','
                          << n_clients << ','
                          << instance.gamma() << ','
                          << instance.deviation() << ','
                          << master.timer().cumulative_time_in_seconds() << ','
                          << separation.timer().cumulative_time_in_seconds() << ','
                          << master.objective_value() << ','
                          << cb.n_solved_separation() << ','
                          << cb.n_generated_scenarios() << ','
                          << std::endl;

                //master.export_model(std::string("master_") + std::to_string(Gamma) + ".sol");
            }

        }

    }

    return 0;
}
