//
// Created by henri on 08/07/22.
//

#ifndef CONVEX_ARO_FLP_RANDOMINSTANCE_H
#define CONVEX_ARO_FLP_RANDOMINSTANCE_H

#include "flp_Instance.h"
#include <random>

namespace flp {
    class RandomInstance;
}

class flp::RandomInstance : public flp::Instance {
    std::random_device m_rd;
    std::mt19937 m_engine;

    std::vector<std::pair<double, double>> m_sites;
    std::vector<std::pair<double, double>> m_clients;

    static double distance(const std::pair<double, double>& t_a, const std::pair<double, double>& t_b);
    void compute_transportation_costs();
    void compute_capacities();
    void compute_activation_costs();
    void compute_demands(double t_ratio);
    void fill_with_n_points(unsigned int t_n, std::vector<std::pair<double, double>>& t_destination);
public:
    RandomInstance(unsigned int t_n_sites, unsigned int t_n_clients, double t_ratio);
};


#endif //CONVEX_ARO_FLP_RANDOMINSTANCE_H
