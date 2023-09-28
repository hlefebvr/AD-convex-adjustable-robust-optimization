//
// Created by henri on 27.09.23.
//
#include <iostream>
#include "modeling.h"
#include "problems/facility-location-problem/FLP_Instance.h"
#include "CCG.h"
#include "GBD.h"
#include "Nominal.h"

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

    std::unique_ptr<AbstractSolver> solver;

    if (method == "GBD") {
        solver = std::make_unique<FLP::GBD>(instance, Gamma, deviation);
    } else if (method == "CCG") {
        solver = std::make_unique<FLP::CCG>(instance, Gamma, deviation);
    } else if (method == "Nominal") {
        solver = std::make_unique<FLP::Nominal>(instance);
    } else {
        throw std::invalid_argument("Allowed values for parameter method are: GBD, CCG, Nominal. Received \" " + method + " \".");
    }

    const auto report = solver->solve(time_limit, 1e-4);

    // Report
    std::cout << "result,"
              << path_to_instance << ","
              << instance.n_facilities() << ","
              << instance.n_customers() << ","
              << Gamma << ","
              << deviation << ","
              << time_limit << ","
              << method << ","
              << report.total_time << ","
              << report.master_time << ","
              << report.separation_time << ","
              << report.best_bound << ","
              << report.iteration_count << ","
              << report.fail_flag
              << std::endl;

    return 0;
}
