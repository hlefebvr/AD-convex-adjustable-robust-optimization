//
// Created by henri on 28.09.23.
//

#ifndef CONVEXADJUSTALBEROBUSTOPTIMIZATION_INSTANCE_H
#define CONVEXADJUSTALBEROBUSTOPTIMIZATION_INSTANCE_H

#include <vector>
#include <iostream>

namespace RAP {
    class Instance;

    Instance read_instance(const std::string& t_path_to_file);
    Instance generate_instance(unsigned int t_n_servers, unsigned int t_n_clients);
}

class RAP::Instance {
    std::vector<double> m_unitary_costs;
    std::vector<std::vector<double>> m_service_rates;
    std::vector<double> m_demands;
    std::vector<double> m_congestion_factors;
public:
    Instance(unsigned int t_n_servers, unsigned int t_n_clients);

    [[nodiscard]] unsigned int n_servers() const { return m_unitary_costs.size(); }
    [[nodiscard]] unsigned int n_clients() const { return m_demands.size(); }

    [[nodiscard]] double unitary_cost(unsigned int t_i) const { return m_unitary_costs[t_i]; }
    void set_unitary_cost(unsigned int t_i, double t_value) { m_unitary_costs[t_i] = t_value; }

    [[nodiscard]] double service_rate(unsigned int t_i, unsigned int t_j) const { return m_service_rates[t_i][t_j]; }
    void set_service_rate(unsigned int t_i, unsigned int t_j, double t_value) { m_service_rates[t_i][t_j] = t_value; }

    [[nodiscard]] double demand(unsigned int t_j) const { return m_demands[t_j]; }
    void set_demand(unsigned int t_j, double t_value) { m_demands[t_j] = t_value; }

    [[nodiscard]] double congestion_factor(unsigned int t_i) const { return m_congestion_factors[t_i]; }
    void set_congestion_factor(unsigned int t_i, double t_value) { m_congestion_factors[t_i] = t_value; }
};

std::ostream& operator<<(std::ostream& t_os, const RAP::Instance& t_instance);

#endif //CONVEXADJUSTALBEROBUSTOPTIMIZATION_INSTANCE_H
