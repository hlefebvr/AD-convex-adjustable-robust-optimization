//
// Created by henri on 28.09.23.
//

#include <fstream>
#include <random>
#include "Instance.h"

RAP::Instance::Instance(unsigned int t_n_servers, unsigned int t_n_clients)
    :
        m_unitary_costs(t_n_servers, 0.),
        m_service_rates(t_n_servers, std::vector<double>(t_n_clients, 0.)),
        m_demands(t_n_clients, 0.),
        m_congestion_factors(t_n_servers, 0.) {

}

RAP::Instance RAP::read_instance(const std::string &t_path_to_file) {

    std::ifstream file(t_path_to_file);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open instance file: " + t_path_to_file + ".");
    }

    unsigned int n_servers, n_clients;
    double placeholder;

    file >> n_servers >> n_clients;

    RAP::Instance result(n_servers, n_clients);

    for (unsigned int i = 0 ; i < n_servers ; ++i) {
        file >> placeholder;
        result.set_unitary_cost(i, placeholder);
        file >> placeholder;
        result.set_congestion_factor(i, placeholder);
    }

    for (unsigned int j = 0 ; j < n_clients ; ++j) {
        file >> placeholder;
        result.set_demand(j, placeholder);
    }

    for (unsigned int i = 0 ; i < n_servers ; ++i) {
        for (unsigned int j = 0 ; j < n_clients ; ++j) {
            file >> placeholder;
            result.set_service_rate(i, j, placeholder);
        }
    }

    file.close();

    return result;
}

RAP::Instance RAP::generate_instance(unsigned int t_n_servers, unsigned int t_n_clients) {

    RAP::Instance result(t_n_servers, t_n_clients);

    std::random_device rd;
    std::mt19937 generator(100);

    std::uniform_real_distribution<double> service_rate_dist(0, 1);
    std::uniform_real_distribution<double> unitary_cost_dist(8, 10);
    std::uniform_real_distribution<double> congestion_factor_dist(0, 1);
    std::uniform_real_distribution<double> demand_dist(1, 50);

    double sum_service_rates = 0;

    for (unsigned int i = 0 ; i < t_n_servers ; ++i) {
        for (unsigned int j = 0 ; j < t_n_clients ; ++j) {
            double service_rate = service_rate_dist(generator);
            result.set_service_rate(i, j, service_rate);
            sum_service_rates += service_rate;
        }
    }

    for (unsigned int i = 0 ; i < t_n_servers ; ++i) {
        result.set_unitary_cost(i, unitary_cost_dist(generator) * sum_service_rates / t_n_clients );
        result.set_congestion_factor(i, congestion_factor_dist(generator));
    }

    for (unsigned int j = 0 ; j < t_n_clients ; ++j) {
        result.set_demand(j, demand_dist(generator));
    }

    return result;
}

std::ostream& operator<<(std::ostream& t_os, const RAP::Instance& t_instance) {

    const unsigned int n_servers = t_instance.n_servers();
    const unsigned int n_clients = t_instance.n_clients();

    t_os << n_servers << '\t' << n_clients << '\n';

    for (unsigned int i = 0 ; i < n_servers ; ++i) {
        t_os << t_instance.unitary_cost(i) << '\t' << t_instance.congestion_factor(i) << '\n';
    }

    for (unsigned int j = 0 ; j < n_clients ; ++j) {
        t_os << t_instance.demand(j) << '\n';
    }

    for (unsigned int i = 0 ; i < n_servers ; ++i) {
        for (unsigned int j = 0 ; j < n_clients ; ++j) {
            t_os << t_instance.service_rate(i, j) << '\t';
        }
        t_os << '\n';
    }

    return t_os;
}