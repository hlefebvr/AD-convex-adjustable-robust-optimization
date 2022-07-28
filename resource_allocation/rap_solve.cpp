#include <iostream>
#include <algorithm>
#include <cmath>
#include "separation/rap_SeparationProblem.h"
#include "master/rap_MasterProblem.h"
#include "../solve.h"
#include "instance/rap_InstanceFromFile.h"

using namespace rap;

int main(int t_argc, const char** t_argv) {

    if (t_argc != 4) { throw std::runtime_error("Expected parameters: <INSTANCE_FILE> <DEVIATION> <ALG=0,1>"); }

    const std::string path = t_argv[1];
    const double deviation = std::atof(t_argv[2]);
    const auto algorithm = (Algorithm) std::atoi(t_argv[3]);

    InstanceFromFile instance(path);

    for (const double percentage_deviations : { .05, .10, .15, .20, .25 }) {

        const double g = std::floor( instance.n_clients() * percentage_deviations );

        instance.set_robust_parameters(g, deviation);

        MasterProblem master(instance);
        SeparationProblem separation(instance);

        std::cout
                << path << ','
                << instance.n_resources() << ','
                << instance.n_clients() << ','
                << g << ','
                << deviation << ',';

        solve_and_report(master, separation, algorithm);

        std::cout << std::endl;

    }


    return 0;
}
