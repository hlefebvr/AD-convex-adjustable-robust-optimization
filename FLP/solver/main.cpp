//
// Created by henri on 27.09.23.
//
#include <iostream>
#include "modeling.h"
#include "problems/facility-location-problem/FLP_Instance.h"
#include "CCG.h"
#include "GBD.h"

int main(int t_argc, const char** t_argv) {

    if (t_argc != 6) {
        throw std::invalid_argument("Arguments: <path_to_instance> <Gamma> <deviation> <method=GBD|CCG> <time_limit>");
    }

    const std::string path_to_instance = t_argv[1];
    const double Gamma = std::stof(t_argv[2]);
    const double deviation = std::stof(t_argv[3]);
    const std::string method = t_argv[4];
    const double time_limit = std::stof(t_argv[5]);

    const auto instance = idol::Problems::FLP::read_instance_1991_Cornuejols_et_al(path_to_instance);

    std::unique_ptr<ConvexAROSolver> solver;

    if (method == "GBD") {
        solver = std::make_unique<FLP::GBD>(instance, Gamma, deviation);
    } else if (method == "CCG") {
        solver = std::make_unique<FLP::CCG>(instance, Gamma, deviation);
    } else {
        throw std::invalid_argument("Allowed values for parameter method are: GBD, CCG. Received \" " + method + " \".");
    }

    solver->solve(time_limit, 1e-4);

    // Report
    std::cout << "result,"
              << method << ","
              << path_to_instance << ","
              << Gamma << ","
              << deviation << ","
              << time_limit << ","
              << std::endl;

    return 0;
}
