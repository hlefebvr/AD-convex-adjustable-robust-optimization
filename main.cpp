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

    const double Gamma = 2;
    const double deviation = .25;

    // InstanceFromFile instance("instance.txt");

    for (unsigned int k = 0 ; k < 20;  k += 1) {

        RandomInstance instance(50, 25, 1.4);

        for (unsigned int i = 0 ; i <= 10 ; i += 2) {

            instance.set_robust_parameters(i, deviation);

            SeparationProblem separation(instance);
            CuttingPlaneCallback cb(instance, separation);

            MasterProblem master(instance);
            master.set_callback(cb);

            auto t_start = std::chrono::high_resolution_clock::now();
            master.solve();
            auto t_end = std::chrono::high_resolution_clock::now();
            double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end-t_start).count();

            std::cout << "RESULT;" << instance.gamma() << ";" << instance.deviation() << ";" << elapsed_time_ms / 1000. << ";" << master.objective_value() << std::endl;

            //master.export_model(std::string("master_") + std::to_string(i) + ".sol");
        }

    }

    return 0;
}
