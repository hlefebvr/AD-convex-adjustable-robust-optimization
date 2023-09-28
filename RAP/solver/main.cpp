//
// Created by henri on 28.09.23.
//
#include <iostream>
#include <cmath>
#include "../instance/Instance.h"
#include "CCG.h"
#include "GBD.h"
#include "Nominal.h"

int main(int t_argc, const char** t_argv) {

    if (t_argc != 6) {
        throw std::invalid_argument("Arguments: <path_to_instance> <p> <deviation> <method=GBD|CCG> <time_limit>");
    }

    const std::string path_to_instance = t_argv[1];
    const double percentage_for_Gamma = std::stof(t_argv[2]);
    const double deviation = std::stof(t_argv[3]);
    const std::string method = t_argv[4];
    const double time_limit = std::stof(t_argv[5]);

    if (percentage_for_Gamma < 0. || percentage_for_Gamma > 1.) {
        throw std::invalid_argument("Argument <p> must be between 0 and 1.");
    }

    //const auto instance = RAP::read_instance(path_to_instance);
    const auto instance = RAP::generate_instance(2, 2);

    const double Gamma = std::floor( percentage_for_Gamma * instance.n_clients() );

    std::unique_ptr<AbstractSolver> solver;

    if (method == "GBD") {
        solver = std::make_unique<RAP::GBD>(instance, Gamma, deviation);
    } else if (method == "CCG") {
        solver = std::make_unique<RAP::CCG>(instance, Gamma, deviation);
    } else if (method == "Nominal") {
        solver = std::make_unique<RAP::Nominal>(instance);
    } else {
        throw std::invalid_argument("Argument <method> must be among GBD, CCG and Nominal. Received \" " + method + " \".");
    }

    const auto report = solver->solve(time_limit, 1e-4);

    // Report
    std::cout << "result,"
              << path_to_instance << ","
              << instance.n_servers() << ","
              << instance.n_clients() << ","
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
