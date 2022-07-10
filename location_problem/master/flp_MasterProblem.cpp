//
// Created by henri on 09/07/22.
//

#include "flp_MasterProblem.h"
#include "../instance/flp_Instance.h"
#include "../cutting_plane/flp_Callback.h"

flp::MasterProblem::MasterProblem(const flp::Instance &t_instance)
        : m_instance(t_instance) {

    create_variables_x();
    create_variable_tau();
    create_objective();

    m_model.set(GRB_IntParam_LazyConstraints, 1);

}

void flp::MasterProblem::create_variables_x() {
    create_variables(m_x, m_instance.n_sites(), 0., 1., GRB_BINARY);
}

void flp::MasterProblem::create_variable_tau() {
    m_tau = m_model.addVar(0.0, GRB_INFINITY, 0., GRB_CONTINUOUS);
}

void flp::MasterProblem::create_objective() {
    const unsigned int n_sites = m_instance.n_sites();

    GRBLinExpr expr = m_tau;
    for (unsigned int i = 0 ; i < n_sites ; i += 1) {
        expr += m_instance.f(i) * m_x[i];
    }

    m_model.setObjective(expr, GRB_MINIMIZE);
}

void flp::MasterProblem::set_callback(Callback &t_cb) {
    m_model.setCallback(&t_cb);
    t_cb.set_model(m_model);
    t_cb.set_variables_x(m_x);
    t_cb.set_variable_tau(m_tau);
}
