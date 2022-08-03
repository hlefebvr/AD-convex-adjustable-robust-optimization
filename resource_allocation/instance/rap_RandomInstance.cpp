//
// Created by henri on 28/07/22.
//

#include <algorithm>
#include "rap_RandomInstance.h"

rap::RandomInstance::RandomInstance(unsigned int t_n_resources, unsigned int t_n_clients)
    : m_engine(m_rd()) {

    m_n_resources = t_n_resources;
    m_n_clients = t_n_clients;

    compute_service_rates();
    compute_costs();
    compute_alphas();
    compute_demands();

}

void rap::RandomInstance::compute_service_rates() {
    std::uniform_real_distribution<double> dist(0., 1.);

    m_mu.resize(m_n_resources);
    for (unsigned int i = 0 ; i < m_n_resources ; i += 1) {
        m_mu[i].reserve(m_n_clients);
        for (unsigned int j = 0 ; j < m_n_clients ; j += 1) {
            m_mu[i].emplace_back(dist(m_engine));
        }
    }
}

void rap::RandomInstance::compute_costs() {
    std::uniform_real_distribution<double> dist(4., 10.);

    m_c.reserve(m_n_resources);
    for (unsigned int i = 0 ; i < m_n_resources ; i += 1) {
        //double avg = std::accumulate(m_mu[i].begin(), m_mu[i].end(), 0.) / m_n_clients;
        const double max = *std::max_element(m_mu[i].begin(), m_mu[i].end());
        m_c.emplace_back(max * dist(m_engine));
    }
}

void rap::RandomInstance::compute_alphas() {
    std::uniform_real_distribution<double> dist(0., 1.);

    m_a.reserve(m_n_resources);
    for (unsigned int i = 0 ; i < m_n_resources ; i += 1) {
        m_a.emplace_back(dist(m_engine));
    }
}

void rap::RandomInstance::compute_demands() {
    std::uniform_real_distribution<double> dist(1., 50.);

    m_d.reserve(m_n_clients);
    for (unsigned int j = 0 ; j < m_n_clients ; j += 1) {
        m_d.emplace_back(dist(m_engine));
    }
}
