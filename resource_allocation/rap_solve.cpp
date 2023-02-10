#include <iostream>
#include <algorithm>
#include <cmath>
#include <sstream>
#include "separation/rap_SeparationProblem.h"
#include "master/rap_MasterProblem.h"
#include "../solve.h"
#include "instance/rap_InstanceFromFile.h"
#include "instance/rap_RandomInstance.h"

using namespace rap;

int main(int t_argc, const char** t_argv) {

    if (t_argc != 2) { throw std::runtime_error("Expected parameters: <INSTANCE_FILE>"); }

    const std::string path = t_argv[1];

    InstanceFromFile instance(path);
    //RandomInstance instance(20, 70);

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
                        << instance.n_resources() << ','
                        << instance.n_clients() << ','
                        << Gamma << ','
                        << uncertainty_percentage << ','
                        << maximum_deviation << ',';

                solve_and_report(output, master, separation, algorithm);

                std::cout << output.str() << std::endl;

            }

        }

    }


    return 0;
}
