//
// Created by henri on 18.06.24.
//
#include <iostream>
#include <cmath>
#include <fstream>
#include <random>
#include "idol/problems/facility-location-problem/FLP_Instance.h"

using namespace idol;

double compute_average_fixed_cost(const Problems::FLP::Instance& t_instance) {
    double result = 0;
    const unsigned int n_facilities = t_instance.n_facilities();
    for (unsigned int i = 0 ; i < n_facilities ; ++i) {
        result += t_instance.fixed_cost(i);
    }
    return result / n_facilities;
}

double compute_average_capacity(const Problems::FLP::Instance& t_instance) {
    double result = 0;
    const unsigned int n_facilities = t_instance.n_facilities();
    for (unsigned int i = 0 ; i < n_facilities ; ++i) {
        result += t_instance.capacity(i);
    }
    return result / n_facilities;
}

double compute_total_demand(const Problems::FLP::Instance& t_instance) {
    double result = 0;
    const unsigned int n_customers = t_instance.n_customers();
    for (unsigned int j = 0 ; j < n_customers ; ++j) {
        result += t_instance.demand(j);
    }
    return result;
}

double compute_average_demand_per_facility(const Problems::FLP::Instance& t_instance) {
    const double total_demand = compute_total_demand(t_instance);
    return total_demand / std::ceil(1.2 * total_demand / compute_average_capacity(t_instance));
}

double compute_max_opening_cost(const Problems::FLP::Instance& t_instance) {
    double result = 0;
    const unsigned int n_facilities = t_instance.n_facilities();
    for (unsigned int i = 0 ; i < n_facilities ; ++i) {
        result = std::max(result, t_instance.fixed_cost(i));
    }
    return result;
}

double compute_disecconomy_of_scale_factor(const Problems::FLP::Instance& t_instance) {
    return .2 * compute_average_fixed_cost(t_instance) * (compute_average_capacity(t_instance) -
            compute_average_demand_per_facility(t_instance) ) / compute_average_demand_per_facility(t_instance);
}

int main(int t_argc, const char** t_argv) {

    if (t_argc != 6) {
        throw std::invalid_argument("Arguments: <destination_folder> <n_facilities> <n_customers> <capacity_over_demand_ratio> <n_instances>");
    }

    std::random_device rd;
    std::mt19937 engine(rd());

    const std::string destination_folder = t_argv[1];
    const auto n_facilities = std::stoul(t_argv[2]);
    const auto n_customers = std::stoul(t_argv[3]);
    const auto capacity_over_demand_ratio = std::stod(t_argv[4]);
    const auto n_instances = std::stoul(t_argv[5]);

    for (unsigned int index = 0 ; index < n_instances ; ++index) {

        const std::string path_to_file = destination_folder
                                         + "/instance_F" + std::to_string(n_facilities)
                                         + "_C" + std::to_string(n_customers)
                                         + "_R" + std::to_string((unsigned int) std::floor(1000 * capacity_over_demand_ratio))
                                         + "__" + std::to_string(index)
                                         + ".txt";

        std::ofstream file(path_to_file);

        if (!file.is_open()) {
            throw std::runtime_error("Could not open destination file: " + path_to_file + ".");
        }

        const auto instance = Problems::FLP::generate_instance_1991_Cornuejols_et_al(n_facilities, n_customers, capacity_over_demand_ratio);

        file << instance;

        const double max_opening_cost = compute_max_opening_cost(instance);
        const double base_diseconomy_of_scale_factor = compute_disecconomy_of_scale_factor(instance);

        for (unsigned int i = 0 ; i < n_facilities ; ++i) {
            double disecconomy_of_scale_factor = base_diseconomy_of_scale_factor * max_opening_cost / (max_opening_cost + instance.fixed_cost(i));
            disecconomy_of_scale_factor *= std::uniform_real_distribution<double>(1.8, 2.2)(engine);
            file << disecconomy_of_scale_factor << '\n';
        }

        file.close();

    }

    return 0;
}
