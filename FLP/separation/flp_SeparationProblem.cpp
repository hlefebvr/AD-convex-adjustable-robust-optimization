//
// Created by henri on 09/07/22.
//

#include <complex>
#include "flp_SeparationProblem.h"
#include "../master/flp_FirstStageProposition.h"

flp::SeparationProblem::SeparationProblem(const flp::Instance &t_instance)
    : m_instance(t_instance) {

    //m_model.set(GRB_DoubleParam_BestBdStop, 1e-2);

    create_variables_pi();
    create_variables_theta();
    create_variables_v();
    create_variables_z();
    create_variables_xi();
    create_variable_mu();

    create_constraint_simplex();
    create_constraints_perspective_conjugate();
    create_constraints_farkas();
    create_constraint_budget();
    create_linearization_constraints();

    create_objective_without_x_and_tau();
}

void flp::SeparationProblem::create_variables_pi() {
    const unsigned int n_sites = m_instance.n_sites();
    const unsigned int n_clients = m_instance.n_clients();
    m_pi.resize(3);
    create_variables(m_pi[0], n_clients, 0.0, 1.0, GRB_CONTINUOUS);
    create_variables(m_pi[1], n_sites, 0.0, 1.0, GRB_CONTINUOUS);
    create_variables(m_pi[2], n_sites, 0.0, 1.0, GRB_CONTINUOUS);
}

void flp::SeparationProblem::create_variables_v() {
    create_variables(m_v, m_instance.n_sites(), 0.0, GRB_INFINITY, GRB_CONTINUOUS);
}

void flp::SeparationProblem::create_variables_z() {
    create_variables(m_z, m_instance.n_clients(), 0.0, 1.0, GRB_CONTINUOUS);
}

void flp::SeparationProblem::create_variables_theta() {
    create_variables(m_theta, m_instance.n_sites(), 0.0, GRB_INFINITY, GRB_CONTINUOUS);
}

void flp::SeparationProblem::create_variables_xi() {
    create_variables(m_xi, m_instance.n_clients(), 0.0, 1.0, GRB_BINARY);
}

void flp::SeparationProblem::create_variable_mu() {
    m_mu = m_model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS);
}

void flp::SeparationProblem::create_constraint_simplex() {
    GRBLinExpr expr = m_mu;
    for (auto& dual_vars : m_pi) {
        for (auto& pi : dual_vars) {
            expr += pi;
        }
    }
    m_model.addConstr(expr == 1);
}

void flp::SeparationProblem::create_constraints_perspective_conjugate() {
    const unsigned int n_sites = m_instance.n_sites();
    for (unsigned int i = 0 ; i < n_sites ; i += 1) {
        auto aux = m_model.addVar(-GRB_INFINITY, GRB_INFINITY, 0.0, GRB_CONTINUOUS);
        m_model.addConstr(aux == m_pi[1][i] - m_pi[2][i] - m_instance.a() * m_mu);
        m_model.addQConstr( aux * aux <= 4 * m_instance.b() * m_mu * m_theta[i] );
    }
}

void flp::SeparationProblem::create_constraints_farkas() {
    const unsigned int n_sites = m_instance.n_sites();
    const unsigned int n_clients = m_instance.n_clients();

    for (unsigned int i = 0 ; i < n_sites ; i += 1) {
        for (unsigned int j = 0 ; j < n_clients ; j += 1) {
            m_model.addConstr(m_pi[0][j] - m_pi[1][i] - m_mu * m_instance.t(i,j) <= 0);
        }
    }
}

void flp::SeparationProblem::create_objective_without_x_and_tau() {
    const unsigned int n_sites = m_instance.n_sites();
    const unsigned int n_clients = m_instance.n_clients();

    GRBLinExpr expr = 0;
    for (unsigned int i = 0 ; i < n_sites ; i += 1) {
        expr += -1. * m_theta[i];
    }
    for (unsigned int j = 0 ; j < n_clients ; j += 1) {
        expr += m_pi[0][j] * m_instance.d(j);
        expr += m_z[j] * m_instance.d(j) * m_instance.deviation();
    }
    m_model.setObjective(expr, GRB_MAXIMIZE);
}

void flp::SeparationProblem::create_constraint_budget() {
    const unsigned int n_clients = m_instance.n_clients();

    GRBLinExpr expr = 0;
    for (unsigned int j = 0 ; j < n_clients ; j += 1) {
        expr += m_xi[j];
    }
    m_model.addConstr(expr <= m_instance.gamma());
}

void flp::SeparationProblem::create_linearization_constraints() {
    const unsigned int n_clients = m_instance.n_clients();

    for (unsigned int j = 0 ; j < n_clients ; j += 1) {
        m_model.addConstr(m_z[j] <= m_xi[j]);
        m_model.addConstr(m_z[j] <= m_pi[0][j]);
        m_model.addConstr(m_z[j] >= m_pi[0][j] + m_xi[j] - 1);
    }
}

void flp::SeparationProblem::update(const FirstStageProposition &t_proposition) {
    const unsigned int n_sites = m_instance.n_sites();

    m_mu.set(GRB_DoubleAttr_Obj, -1.0 * t_proposition.tau());

    for (unsigned int i = 0 ; i < n_sites ; i += 1) {
        m_pi[2][i].set(GRB_DoubleAttr_Obj, -1.0 * m_instance.q(i) * t_proposition.x(i));
    }
}

flp::RobustCertificate flp::SeparationProblem::get_certificate() const {
    RobustCertificate result(m_instance);

    const unsigned int n_sites = m_instance.n_sites();
    const unsigned int n_clients = m_instance.n_clients();

    result.set_objective_value(m_model.get(GRB_DoubleAttr_ObjVal));

    for (unsigned int j = 0 ; j < n_clients ; j += 1) {
        result.set_pi_value(0, j, m_pi[0][j].get(GRB_DoubleAttr_X));
        result.set_xi_value(j, m_xi[j].get(GRB_DoubleAttr_X));
    }
    for (unsigned int i = 0 ; i < n_sites ; i += 1) {
        result.set_pi_value(1, i, m_pi[1][i].get(GRB_DoubleAttr_X));
        result.set_pi_value(2, i, m_pi[2][i].get(GRB_DoubleAttr_X));
        result.set_v_value(i, m_v[i].get(GRB_DoubleAttr_X));
        result.set_theta_value(i, m_theta[i].get(GRB_DoubleAttr_X));
    }
    result.set_mu_value(m_mu.get(GRB_DoubleAttr_X));

    return result;
}
