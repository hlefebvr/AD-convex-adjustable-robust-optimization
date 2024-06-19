//
// Created by henri on 18.06.24.
//
#include <iostream>
#include "idol/modeling.h"
#include "CCG.h"
#include "GBD.h"
#include "Nominal.h"

int main(int t_argc, const char** t_argv) {

    if (t_argc != 6) {
        throw std::invalid_argument("Arguments: <path_to_instance> <p> <deviation> <method=GBD|CCG|Nominal> <time_limit>");
    }

    const std::string path_to_instance = t_argv[1];
    const double percentage_for_Gamma = std::stof(t_argv[2]);
    const double deviation = std::stof(t_argv[3]);
    const std::string method = t_argv[4];
    const double time_limit = std::stof(t_argv[5]);

    if (percentage_for_Gamma < 0. || percentage_for_Gamma > 1.) {
        throw std::invalid_argument("Argument <p> must be between 0 and 1.");
    }

    const auto instance = FLP::Instance::read_from_file(path_to_instance);

    double total_demand = 0;
    double total_capacity = 0;
    for (unsigned int i = 0; i < instance.n_facilities(); ++i) {
        total_capacity += instance.capacity(i);
    }
    for (unsigned int j = 0; j < instance.n_customers(); ++j) {
        total_demand += instance.demand(j);
    }
    std::cout << "Total demand: " << total_demand << std::endl;
    std::cout << "Total capacity: " << total_capacity << std::endl;

    const double Gamma = std::floor( percentage_for_Gamma * instance.n_customers() );

    std::unique_ptr<AbstractSolver> solver;

    if (method == "GBD") {
        throw idol::Exception("GBD not implemented.");
        // solver = std::make_unique<FLP::GBD>(instance, Gamma, deviation);
    } else if (method == "CCG") {
        solver = std::make_unique<FLP::CCG>(instance, Gamma, deviation);
    } else if (method == "Nominal") {
        solver = std::make_unique<FLP::Nominal>(instance);
    } else {
        throw std::invalid_argument("Argument <method> must be among GBD, CCG and Nominal. Received \" " + method + " \".");
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
