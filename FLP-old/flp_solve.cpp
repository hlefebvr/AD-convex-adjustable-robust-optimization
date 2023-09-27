#include <iostream>
#include <algorithm>
#include <cmath>
#include <sstream>
#include "separation/flp_SeparationProblem.h"
#include "master/flp_MasterProblem.h"
#include "../solve.h"
#include "instance/flp_InstanceFromFile.h"

using namespace flp;

int main(int t_argc, const char** t_argv) {

    if (t_argc != 2) { throw std::runtime_error("Expected parameters: <INSTANCE_FILE>"); }

    const std::string path = t_argv[1];

    InstanceFromFile instance(path);

    for (const double uncertainty_percentage : { .05, .10, .20 }) {

        for (const double maximum_deviation : { .10, .05 }) {

            for (const Algorithm algorithm : { CCG, GBD }) {

                const double Gamma = std::floor(instance.n_clients() * uncertainty_percentage);

                instance.set_robust_parameters(Gamma, maximum_deviation);

                MasterProblem master(instance);
                SeparationProblem separation(instance);

                std::stringstream output;
                output
                        << path << ','
                        << instance.n_sites() << ','
                        << instance.n_clients() << ','
                        << Gamma << ','
                        << uncertainty_percentage << ','
                        << maximum_deviation << ',';

                solve_and_report(output, master, separation, algorithm, true);

                std::cout << output.str() << std::endl;

            }

        }

    }


    return 0;
}
