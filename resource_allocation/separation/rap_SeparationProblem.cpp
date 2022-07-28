//
// Created by henri on 28/07/22.
//

#include "rap_SeparationProblem.h"
#include "../instance/rap_Instance.h"
#include "../master/rap_FirstStageProposition.h"

rap::SeparationProblem::SeparationProblem(const rap::Instance &t_instance) : m_instance(t_instance) {
    create_variables_pi();
    create_variables_theta();
    create_variables_lambda();
    create_variables_z();
    create_variables_xi();

    create_constraint_simplex();
    create_constraints_perspective_conjugate();
    create_constraints_farkas();
    create_constraint_budget();
    create_linearization_constraints();

    create_objective_without_x();
}

void rap::SeparationProblem::update(const rap::FirstStageProposition &t_proposition) {
    const unsigned int n_resources = m_instance.n_resources();

    for (unsigned int i = 0 ; i < n_resources ; i += 1) {
        m_lambda[i].set(GRB_DoubleAttr_Obj, -1.0 * t_proposition.x(i));
    }
}

rap::RobustCertificate rap::SeparationProblem::get_certificate() const {
    RobustCertificate result(m_instance);

    const unsigned int n_resources = m_instance.n_resources();
    const unsigned int n_clients = m_instance.n_clients();

    result.set_objective_value(m_model.get(GRB_DoubleAttr_ObjVal));

    for (unsigned int j = 0 ; j < n_clients ; j += 1) {
        result.set_pi_value(0, j, m_pi[0][j].get(GRB_DoubleAttr_X));
        result.set_xi_value(j, m_xi[j].get(GRB_DoubleAttr_X));
    }
    for (unsigned int i = 0 ; i < n_resources ; i += 1) {
        result.set_pi_value(1, i, m_pi[1][i].get(GRB_DoubleAttr_X));
        result.set_pi_value(2, i, m_pi[2][i].get(GRB_DoubleAttr_X));
        result.set_lambda_value(i, m_lambda[i].get(GRB_DoubleAttr_X));
        result.set_theta_value(i, m_theta[i].get(GRB_DoubleAttr_X));
    }

    return result;
}

void rap::SeparationProblem::create_variables_pi() {
    const unsigned int n_resources = m_instance.n_resources();
    const unsigned int n_clients = m_instance.n_clients();
    m_pi.resize(2);
    create_variables(m_pi[0], n_clients, 0.0, 1.0, GRB_CONTINUOUS);
    create_variables(m_pi[1], n_resources, 0.0, 1.0, GRB_CONTINUOUS);
}

void rap::SeparationProblem::create_variables_theta() {
    create_variables(m_theta, m_instance.n_resources(), 0.0, GRB_INFINITY, GRB_CONTINUOUS);
}

void rap::SeparationProblem::create_variables_lambda() {
    create_variables(m_lambda, m_instance.n_resources(), 0.0, GRB_INFINITY, GRB_CONTINUOUS);
}

void rap::SeparationProblem::create_variables_z() {
    create_variables(m_z, m_instance.n_clients(), 0.0, 1.0, GRB_CONTINUOUS);
}

void rap::SeparationProblem::create_variables_xi() {
    create_variables(m_xi, m_instance.n_clients(), 0.0, 1.0, GRB_BINARY);
}

void rap::SeparationProblem::create_constraint_simplex() {
    GRBLinExpr expr;
    for (auto& dual_vars : m_pi) {
        for (auto& pi : dual_vars) {
            expr += pi;
        }
    }
    m_model.addConstr(expr == 1);
}

void rap::SeparationProblem::create_constraints_perspective_conjugate() {
    const unsigned int n_sites = m_instance.n_resources();
    for (unsigned int i = 0 ; i < n_sites ; i += 1) {
        auto aux = m_model.addVar(-GRB_INFINITY, GRB_INFINITY, 0.0, GRB_CONTINUOUS);
        m_model.addConstr(aux == m_pi[1][i] - m_lambda[i]);
        m_model.addQConstr( aux * aux <= 4 * m_instance.a(i) * m_lambda[i] * m_theta[i] );
    }
}

void rap::SeparationProblem::create_constraints_farkas() {
    const unsigned int n_resources = m_instance.n_resources();
    const unsigned int n_clients = m_instance.n_clients();

    for (unsigned int i = 0 ; i < n_resources ; i += 1) {
        for (unsigned int j = 0 ; j < n_clients ; j += 1) {
            m_model.addConstr(m_pi[0][j] - m_pi[1][i] <= 0);
        }
    }
}

void rap::SeparationProblem::create_constraint_budget() {
    const unsigned int n_clients = m_instance.n_clients();

    GRBLinExpr expr = 0;
    for (unsigned int j = 0 ; j < n_clients ; j += 1) {
        expr += m_xi[j];
    }
    m_model.addConstr(expr <= m_instance.gamma());
}

void rap::SeparationProblem::create_linearization_constraints() {
    const unsigned int n_clients = m_instance.n_clients();

    for (unsigned int j = 0 ; j < n_clients ; j += 1) {
        m_model.addConstr(m_z[j] <= m_xi[j]);
        m_model.addConstr(m_z[j] <= m_pi[0][j]);
        m_model.addConstr(m_z[j] >= m_pi[0][j] + m_xi[j] - 1);
    }
}

void rap::SeparationProblem::create_objective_without_x() {
    const unsigned int n_resources = m_instance.n_resources();
    const unsigned int n_clients = m_instance.n_clients();

    GRBLinExpr expr = 0;
    for (unsigned int i = 0 ; i < n_resources ; i += 1) {
        expr += -1. * m_theta[i];
    }
    for (unsigned int j = 0 ; j < n_clients ; j += 1) {
        expr += m_pi[0][j] * m_instance.d(j);
        expr += m_z[j] * m_instance.d(j) * m_instance.deviation();
    }
    m_model.setObjective(expr, GRB_MAXIMIZE);
}
