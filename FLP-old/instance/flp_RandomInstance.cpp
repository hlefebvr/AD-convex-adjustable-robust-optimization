//
// Created by henri on 08/07/22.
//

#include <algorithm>
#include <stdexcept>
#include "flp_RandomInstance.h"

flp::RandomInstance::RandomInstance(unsigned int t_n_sites, unsigned int t_n_clients, double t_ratio)
    : m_engine(m_rd()) {

    if (t_ratio <= 1.) {
        throw std::runtime_error("Ratio between capacity and demand should be greater than one.");
    }

    m_n_sites = t_n_sites;
    m_n_clients = t_n_clients;
    fill_with_n_points(t_n_sites, m_sites);
    fill_with_n_points(t_n_clients, m_clients);
    compute_transportation_costs();
    compute_capacities();
    compute_activation_costs();
    compute_demands(t_ratio);

}

void flp::RandomInstance::fill_with_n_points(unsigned int t_n, std::vector<std::pair<double, double>> &t_destination) {
    std::uniform_real_distribution<double> dist(0., 1.);

    t_destination.reserve(t_n);
    for (unsigned int i = 0 ; i < t_n ; ++i) {
        t_destination.emplace_back(std::make_pair(dist(m_engine), dist(m_engine)));
    }
}

void flp::RandomInstance::compute_transportation_costs() {

    m_t.resize(m_n_sites);
    for (unsigned int i = 0 ; i < m_n_sites ; i += 1) {
        m_t[i].reserve(m_n_clients);
        for (unsigned int j = 0 ; j < m_n_clients ; j += 1) {
            m_t[i].emplace_back(10. * distance(m_sites[i], m_clients[j]));
        }
    }

}

double flp::RandomInstance::distance(const std::pair<double, double> &t_a, const std::pair<double, double> &t_b) {
    return std::sqrt( std::pow(t_a.first - t_b.first, 2) + std::pow(t_a.second - t_b.second, 2) );
}

void flp::RandomInstance::compute_capacities() {
    std::uniform_real_distribution<double> dist(10., 160.);

    m_q.reserve(m_n_sites);
    for (unsigned int i = 0 ; i < m_n_sites ; ++i) {
        m_q.emplace_back(dist(m_engine));
    }
}

void flp::RandomInstance::compute_activation_costs() {
    std::uniform_real_distribution<double> dist1(0., 90.);
    std::uniform_real_distribution<double> dist2(100., 110.);

    m_f.reserve(m_n_sites);
    for (unsigned int i = 0 ; i < m_n_sites ; ++i) {
        m_f.emplace_back(dist1(m_engine) + dist2(m_engine) * std::sqrt(m_q[i]));
        // if (m_n_sites * m_n_clients <= 500) { m_f.back() *= 2.; }
    }
}

void flp::RandomInstance::compute_demands(double t_ratio) {

    std::uniform_real_distribution<double> dist(0., 1.);

    m_d.reserve(m_n_clients);

    // generate random U(0,1) for weights repartition of demands
    double sum_d_j = 0;
    for (unsigned int i = 0 ; i < m_n_clients ; ++i) {
        m_d.emplace_back(dist(m_engine));
        sum_d_j += m_d.back();
    }

    // scale
    double sum_q_i = std::accumulate(m_q.begin(), m_q.end(), 0.);
    const double scaling_factor = sum_q_i / (t_ratio * sum_d_j);
    std::for_each(m_d.begin(), m_d.end(), [scaling_factor](double& d_j){ d_j *= scaling_factor; });
}
