//
// Created by henri on 28.09.23.
//
#include <iostream>
#include <cmath>
#include <fstream>
#include "problems/facility-location-problem/FLP_Instance.h"

using namespace idol;

int main(int t_argc, const char** t_argv) {

    if (t_argc != 6) {
        throw std::invalid_argument("Arguments: <destination_folder> <n_facilities> <n_customers> <capacity_over_demand_ratio> <n_instances>");
    }

    const std::string destination_folder = t_argv[1];
    const auto n_facilities = std::stoul(t_argv[2]);
    const auto n_customers = std::stoul(t_argv[3]);
    const auto capacity_over_demand_ratio = std::stod(t_argv[4]);
    const auto n_instances = std::stoul(t_argv[5]);

    for (unsigned int index = 0 ; index < n_instances ; ++index) {

        const std::string path_to_file = destination_folder
                + "/instance_F" + std::to_string(n_facilities)
                + "_C" + std::to_string(n_customers)
                + "_R" + std::to_string(std::floor(1000 * capacity_over_demand_ratio))
                + "__" + std::to_string(index)
                + ".txt";

        std::ofstream file(path_to_file);

        if (!file.is_open()) {
            throw std::runtime_error("Could not open destination file: " + path_to_file + ".");
        }

        const auto instance = Problems::FLP::generate_instance_1991_Cornuejols_et_al(n_facilities, n_customers, capacity_over_demand_ratio);

        file << instance;

        file.close();

    }

    return 0;
}
