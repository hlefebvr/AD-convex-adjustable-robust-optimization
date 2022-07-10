//
// Created by henri on 09/07/22.
//

#ifndef CONVEX_ARO_FLP_CALLBACK_H
#define CONVEX_ARO_FLP_CALLBACK_H

#include <gurobi_c++.h>
#include "../master/flp_FirstStageProposition.h"

namespace flp {
    class Callback;
    class SeparationProblem;
    class Instance;
    class RobustCertificate;
}

class flp::Callback : public GRBCallback {
protected:
    SeparationProblem& m_separation;
    const Instance& m_instance;
    const double m_tolerance = 1e-6;

    unsigned int m_n_solved_separation = 0;
    unsigned int m_n_generated_scenarios = 0;

    GRBModel* m_model;
    std::vector<GRBVar> m_x;
    GRBVar m_tau;
    FirstStageProposition get_proposition();

    void callback() final;
    virtual void add_cut(const FirstStageProposition& t_proposition, const RobustCertificate& t_certificate) = 0;
public:
    Callback(const Instance& t_instance, SeparationProblem& t_separation);

    void set_model(GRBModel& t_model) { m_model = &t_model; }
    void set_variables_x(const std::vector<GRBVar>& t_x) { m_x = t_x; }
    void set_variable_tau(const GRBVar& t_tau) { m_tau = t_tau; }

    [[nodiscard]] unsigned int n_generated_scenarios() const { return m_n_generated_scenarios; }
    [[nodiscard]] unsigned int n_solved_separation() const { return m_n_solved_separation; }
};

#endif //CONVEX_ARO_FLP_CALLBACK_H
