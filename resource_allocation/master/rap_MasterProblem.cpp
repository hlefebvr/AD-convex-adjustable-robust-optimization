//
// Created by henri on 28/07/22.
//

#include <algorithm>
#include <cmath>
#include "rap_MasterProblem.h"
#include "../instance/rap_Instance.h"
#include "../separation/rap_RobustCertificate.h"

rap::MasterProblem::MasterProblem(const rap::Instance &t_instance) : m_instance(t_instance) {
    create_variables_x();
    create_objective();
    create_feasibility_constraint();
}

void rap::MasterProblem::create_variables_x() {
    create_variables(m_x, m_instance.n_resources(), 0., GRB_INFINITY, GRB_CONTINUOUS);
}

void rap::MasterProblem::create_objective() {
    const unsigned int n_resources = m_instance.n_resources();

    GRBLinExpr expr = 0;
    for (unsigned int i = 0 ; i < n_resources ; i += 1) {
        expr += m_instance.c(i) * m_x[i];
    }

    m_model.setObjective(expr, GRB_MINIMIZE);
}

rap::FirstStageProposition rap::MasterProblem::get_proposition() {
    FirstStageProposition result(m_instance);

    result.set_objective_value(m_model.get(GRB_DoubleAttr_ObjVal));

    for (unsigned int i = 0, n_resources = m_instance.n_resources() ; i < n_resources ; i += 1) {
        result.set_x_value(i, m_x[i].get(GRB_DoubleAttr_X));
    }

    return result;
}

void rap::MasterProblem::add_benders_cut(const rap::RobustCertificate &t_certificate) {

    const unsigned int n_resources = m_instance.n_resources();
    const unsigned int n_clients = m_instance.n_clients();

    GRBLinExpr expr;

    for (unsigned int i = 0 ; i < n_resources ; ++i) {
        expr += -1. * t_certificate.lambda(i) * m_x[i];
    }

    double offset = 0.;
    for (unsigned int i = 0 ; i < n_resources ; ++i) {
        offset += -1. * t_certificate.theta(i);
    }
    for (unsigned int j = 0 ; j < n_clients ; ++j) {
        offset += t_certificate.pi(0, j) * (m_instance.d(j) + m_instance.d(j) * m_instance.deviation() * t_certificate.xi(j));
    }

    m_model.addConstr(offset + expr <= 0);

}

void rap::MasterProblem::add_scenario_variables(const rap::RobustCertificate &t_certificate) {

    const unsigned int n_resources = m_instance.n_resources();
    const unsigned int n_clients = m_instance.n_clients();

    std::vector<std::vector<GRBVar>> y;
    std::vector<GRBVar> v;
    create_variables(y, n_resources, n_clients, 0, GRB_INFINITY, GRB_CONTINUOUS);
    create_variables(v, n_resources, 0, GRB_INFINITY, GRB_CONTINUOUS);

    create_constraints_v_definition(y, v);
    create_constraints_capacity(v);
    create_constraints_demand(y, t_certificate);

}

void
rap::MasterProblem::create_constraints_v_definition(std::vector<std::vector<GRBVar>> &t_y, std::vector<GRBVar> &t_v) {

    const unsigned int n_resources = m_instance.n_resources();
    const unsigned int n_clients = m_instance.n_clients();

    for (unsigned int i = 0 ; i < n_resources ; i += 1) {

        GRBLinExpr expr;

        for (unsigned int j = 0 ; j < n_clients ; ++j) {
            expr += t_y[i][j];
        }

        m_model.addConstr(t_v[i] >= expr);

    }

}

void rap::MasterProblem::create_constraints_capacity(std::vector<GRBVar> &t_v) {
    const unsigned int n_resources = m_instance.n_resources();

    for (unsigned int i = 0 ; i < n_resources ; i += 1) {

        m_model.addQConstr(t_v[i] + m_instance.a(i) * t_v[i] * t_v[i] <= m_x[i]);

    }
}

void rap::MasterProblem::create_constraints_demand(std::vector<std::vector<GRBVar>> &t_y, const rap::RobustCertificate &t_certificate) {
    const unsigned int n_resources = m_instance.n_resources();
    const unsigned int n_clients = m_instance.n_clients();

    for (unsigned int j = 0 ; j < n_clients ; j += 1) {

        GRBQuadExpr expr;

        for (unsigned int i = 0 ; i < n_resources ; i += 1) {
            expr += m_instance.mu(i,j) * t_y[i][j];
        }

        m_model.addConstr(expr >= m_instance.d(j) + t_certificate.xi(j) * m_instance.d(j) * m_instance.deviation());

    }
}

void rap::MasterProblem::create_feasibility_constraint() {
    const unsigned int n_resources = m_instance.n_resources();
    const unsigned int n_clients = m_instance.n_clients();

    GRBLinExpr expr;

    for (unsigned int i = 0 ; i < n_resources ; i += 1) {
        expr +=  m_x[i];
    }

    double worst_demand = 0.;
    std::vector<double> demands; demands.reserve(m_instance.n_clients());
    for (unsigned int j = 0 ; j < n_clients ; j += 1) {
        worst_demand += m_instance.d(j);
        demands.emplace_back(m_instance.d(j));
    }
    std::sort(demands.begin(), demands.end());
    unsigned int K = std::ceil(m_instance.gamma());
    for (unsigned int k = 0 ; k < K ; k += 1) {
        worst_demand += demands[k] * m_instance.deviation();
    }

    m_model.addConstr(expr >= worst_demand);
}
