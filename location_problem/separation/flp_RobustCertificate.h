//
// Created by henri on 09/07/22.
//

#ifndef CONVEX_ARO_FLP_ROBUSTCERTIFICATE_H
#define CONVEX_ARO_FLP_ROBUSTCERTIFICATE_H

#include <vector>
#include "../instance/flp_Instance.h"

namespace flp {
    class RobustCertificate;
}

class flp::RobustCertificate {
    double m_objective_value = 0.;
    std::vector<std::vector<double>> m_pi;
    std::vector<double> m_theta;
    std::vector<double> m_v;
    std::vector<double> m_xi;
    double m_mu = 0;
public:
    explicit RobustCertificate(const Instance& t_instance)
        : m_pi(3),
          m_v(t_instance.n_sites()),
          m_theta(t_instance.n_sites()),
          m_xi(t_instance.n_clients()) {

        m_pi[0].resize(t_instance.n_clients());
        m_pi[1].resize(t_instance.n_sites());
        m_pi[2].resize(t_instance.n_sites());

    }

    [[nodiscard]] double mu() const { return m_mu; }
    void set_mu_value(double t_v) { m_mu = t_v; }
    [[nodiscard]] double pi(unsigned int t_k, unsigned int t_l) const { return m_pi[t_k][t_l]; }
    void set_pi_value(unsigned int t_k, unsigned int t_l, double t_v) { m_pi[t_k][t_l] = t_v; }
    [[nodiscard]] double v(unsigned int t_i) const { return m_v[t_i]; }
    void set_v_value(unsigned int t_i, double t_v) { m_v[t_i] = t_v; }
    [[nodiscard]] double theta(unsigned int t_i) const { return m_theta[t_i]; }
    void set_theta_value(unsigned int t_i, double t_v) { m_theta[t_i] = t_v; }
    [[nodiscard]] double xi(unsigned int t_j) const { return m_xi[t_j]; }
    void set_xi_value(unsigned int t_j, double t_v) { m_xi[t_j] = t_v; }
    [[nodiscard]] double objective_value() const { return m_objective_value; }
    void set_objective_value(double t_v) { m_objective_value = t_v; }
};


#endif //CONVEX_ARO_FLP_ROBUSTCERTIFICATE_H
