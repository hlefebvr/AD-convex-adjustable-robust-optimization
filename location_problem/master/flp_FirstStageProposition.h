//
// Created by henri on 08/07/22.
//

#ifndef CONVEX_ARO_FLP_FIRSTSTAGEPROPOSITION_H
#define CONVEX_ARO_FLP_FIRSTSTAGEPROPOSITION_H

#include <vector>
#include "../instance/flp_Instance.h"

namespace flp {
    class FirstStageProposition;
}

class flp::FirstStageProposition {
    std::vector<double> m_x_values;
    double m_tau_value = 0;
public:
    explicit FirstStageProposition(const flp::Instance& t_instance) : m_x_values(t_instance.n_sites()) {}

    double x(unsigned int t_i) const { return m_x_values[t_i]; }
    void set_x_value(unsigned int t_i, double t_v) { m_x_values[t_i] = t_v; }
    double tau() const { return m_tau_value; }
    void set_tau_value(double t_v) { m_tau_value = t_v; }
};

#endif //CONVEX_ARO_FLP_FIRSTSTAGEPROPOSITION_H
