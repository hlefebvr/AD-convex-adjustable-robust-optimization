//
// Created by henri on 08/07/22.
//

#ifndef CONVEX_ARO_FLP_INSTANCE_H
#define CONVEX_ARO_FLP_INSTANCE_H

#include <vector>
#include <string>

namespace flp {
    class Instance;
}

std::ostream& operator<<(std::ostream& t_os, const flp::Instance& t_instance);

class flp::Instance {
protected:
    unsigned int m_n_sites {0};
    unsigned int m_n_clients {0};

    std::vector<double> m_f;
    std::vector<double> m_q;
    double m_a;
    double m_b;
    std::vector<double> m_d;
    std::vector<std::vector<double>> m_t;

    friend std::ostream& ::operator<<(std::ostream& t_os, const flp::Instance& t_instance);

    Instance() = default;
public:
    Instance(const Instance&) = delete;
    Instance(Instance&&) = delete;
    Instance& operator=(const Instance&) = delete;
    Instance& operator=(Instance&&) = delete;

    unsigned int n_sites() const { return m_n_sites; }
    unsigned int n_clients() const { return m_n_clients; }
    [[nodiscard]] double f(unsigned int t_i) const { return m_f[t_i]; }
    [[nodiscard]] double q(unsigned int t_i) const { return m_q[t_i]; }
    double a() const { return m_a; }
    double b() const { return m_b; }
    [[nodiscard]] double d(unsigned int t_j) const { return m_d[t_j]; }
    [[nodiscard]] double t(unsigned int t_i, unsigned int t_j) const { return m_t[t_i][t_j]; }
};

#endif //CONVEX_ARO_FLP_INSTANCE_H
