//
// Created by henri on 28/07/22.
//

#ifndef CONVEX_ARO_RAP_ROBUSTCERTIFICATE_H
#define CONVEX_ARO_RAP_ROBUSTCERTIFICATE_H

#include <vector>

namespace rap {
    class RobustCertificate;
    class Instance;
}

class rap::RobustCertificate {
    double m_objective_value = 0.;
    std::vector<std::vector<double>> m_pi;
    std::vector<double> m_theta;
    std::vector<double> m_lambda;
    std::vector<double> m_xi;
public:
    explicit RobustCertificate(const Instance& t_instance);

    [[nodiscard]] double pi(unsigned int t_k, unsigned int t_l) const { return m_pi[t_k][t_l]; }
    void set_pi_value(unsigned int t_k, unsigned int t_l, double t_v) { m_pi[t_k][t_l] = t_v; }
    [[nodiscard]] double lambda(unsigned int t_i) const { return m_lambda[t_i]; }
    void set_v_value(unsigned int t_i, double t_v) { m_lambda[t_i] = t_v; }
    [[nodiscard]] double theta(unsigned int t_i) const { return m_theta[t_i]; }
    void set_theta_value(unsigned int t_i, double t_v) { m_theta[t_i] = t_v; }
    [[nodiscard]] double xi(unsigned int t_j) const { return m_xi[t_j]; }
    void set_xi_value(unsigned int t_j, double t_v) { m_xi[t_j] = t_v; }
    [[nodiscard]] double objective_value() const { return m_objective_value; }
    void set_objective_value(double t_v) { m_objective_value = t_v; }
};


#endif //CONVEX_ARO_RAP_ROBUSTCERTIFICATE_H
