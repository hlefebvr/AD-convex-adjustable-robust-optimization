//
// Created by henri on 28/07/22.
//

#ifndef CONVEX_ARO_RAP_INSTANCE_H
#define CONVEX_ARO_RAP_INSTANCE_H

#include <vector>
#include <ostream>

namespace rap {
    class Instance;
}

std::ostream& operator<<(std::ostream& t_os, const rap::Instance& t_instance);

class rap::Instance {
protected:
    unsigned int m_n_resources {0};
    unsigned int m_n_clients {0};
    std::vector<double> m_c;
    std::vector<double> m_d;
    std::vector<double> m_a;
    std::vector<std::vector<double>> m_mu;

    double m_gamma = 0.;
    double m_deviation = 0.;

    friend std::ostream& ::operator<<(std::ostream& t_os, const rap::Instance& t_instance);

    Instance() = default;
public:
    Instance(const Instance&) = delete;
    Instance(Instance&&) = delete;
    Instance& operator=(const Instance&) = delete;
    Instance& operator=(Instance&&) = delete;

    [[nodiscard]] unsigned int n_resources() const { return m_n_resources;}
    [[nodiscard]] unsigned int n_clients() const { return m_n_clients;}
    [[nodiscard]] double c(unsigned int t_i) const { return m_c[t_i]; }
    [[nodiscard]] double d(unsigned int t_j) const { return m_d[t_j]; }
    [[nodiscard]] double a(unsigned int t_i) const { return m_a[t_i]; }
    [[nodiscard]] double mu(unsigned int t_i, unsigned int t_j) const { return m_mu[t_i][t_j]; }
    double gamma() const { return m_gamma; }
    double deviation() const { return m_deviation; }

    void set_robust_parameters(double t_gamma, double t_deviation) {
        m_gamma = t_gamma;
        m_deviation = t_deviation;
    }
};


#endif //CONVEX_ARO_RAP_INSTANCE_H
