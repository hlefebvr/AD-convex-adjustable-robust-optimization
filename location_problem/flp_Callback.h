//
// Created by henri on 09/07/22.
//

#ifndef CONVEX_ARO_FLP_CALLBACK_H
#define CONVEX_ARO_FLP_CALLBACK_H

#include <gurobi_c++.h>
#include "flp_FirstStageProposition.h"

namespace flp {
    class Callback;
    class SeparationProblem;
    class Instance;
}

class flp::Callback : public GRBCallback {
protected:
    SeparationProblem& m_separation;
    const Instance& m_instance;
    const double m_tolerance = 1e-6;

    std::vector<GRBVar> m_x;
    GRBVar m_tau;
    FirstStageProposition get_proposition();
public:
    Callback(Instance& t_instance, SeparationProblem& t_separation);

    void set_variables_x(const std::vector<GRBVar>& t_x) { m_x = t_x; }
    void set_variable_tau(const GRBVar& t_tau) { m_tau = t_tau; }
};

#endif //CONVEX_ARO_FLP_CALLBACK_H
