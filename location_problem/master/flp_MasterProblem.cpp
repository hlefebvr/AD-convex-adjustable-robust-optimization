//
// Created by henri on 09/07/22.
//

#include "flp_MasterProblem.h"
#include "../instance/flp_Instance.h"
#include "../callback_cutting_plane/flp_Callback.h"
#include "../separation/flp_RobustCertificate.h"
#include <algorithm>
#include <cmath>

flp::MasterProblem::MasterProblem(const flp::Instance &t_instance)
        : m_instance(t_instance) {

    create_variables_x();
    create_variable_tau();
    create_feasibility_constraint();
    create_objective();

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
    m_model.set(GRB_IntParam_LazyConstraints, 1);
    m_model.setCallback(&t_cb);
    t_cb.set_model(m_model);
    t_cb.set_variables_x(m_x);
    t_cb.set_variable_tau(m_tau);
}

flp::FirstStageProposition flp::MasterProblem::get_proposition() {
    FirstStageProposition result(m_instance);

    for (unsigned int i = 0, n_sites = m_instance.n_sites() ; i < n_sites ; i += 1) {
        result.set_x_value(i, m_x[i].get(GRB_DoubleAttr_X));
    }

    result.set_tau_value(m_tau.get(GRB_DoubleAttr_X));

    return result;
}

void flp::MasterProblem::add_benders_cut(const flp::RobustCertificate &t_certificate) {

    const unsigned int n_sites = m_instance.n_sites();
    const unsigned int n_clients = m_instance.n_clients();

    GRBLinExpr expr = -1. * t_certificate.mu() * m_tau;

    for (unsigned int i = 0 ; i < n_sites ; ++i) {
        expr += -1. * m_instance.q(i) * t_certificate.pi(2, i) * m_x[i];
    }

    double offset = 0.;
    for (unsigned int i = 0 ; i < n_sites ; ++i) {
        offset += -1. * t_certificate.theta(i);
    }
    for (unsigned int j = 0 ; j < n_clients ; ++j) {
        offset += t_certificate.pi(0, j) * (m_instance.d(j) + m_instance.d(j) * m_instance.deviation() * t_certificate.xi(j));
    }

    m_model.addConstr(offset + expr <= 0);

}

void flp::MasterProblem::add_scenario_variables(const flp::RobustCertificate &t_certificate) {

    const unsigned int n_sites = m_instance.n_sites();
    const unsigned int n_clients = m_instance.n_clients();

    std::vector<std::vector<GRBVar>> y;
    std::vector<GRBVar> v;
    create_variables(y, n_sites, n_clients, 0, GRB_INFINITY, GRB_CONTINUOUS);
    create_variables(v, n_sites, 0, GRB_INFINITY, GRB_CONTINUOUS);

    create_constraints_v_definition(y, v);
    create_constraints_capacity(y, v);
    create_constraints_demand(y, v, t_certificate);
    create_constraints_objective(y, v);

}

void
flp::MasterProblem::create_constraints_v_definition(std::vector<std::vector<GRBVar>> &t_y, std::vector<GRBVar> &t_v) {
    const unsigned int n_sites = m_instance.n_sites();
    const unsigned int n_clients = m_instance.n_clients();

    for (unsigned int i = 0 ; i < n_sites ; i += 1) {

        GRBLinExpr expr;

        for (unsigned int j = 0 ; j < n_clients ; ++j) {
            expr += t_y[i][j];
        }

        m_model.addConstr(t_v[i] >= expr);

    }
}

void flp::MasterProblem::create_constraints_capacity(std::vector<std::vector<GRBVar>> &t_y, std::vector<GRBVar> &t_v) {
    const unsigned int n_sites = m_instance.n_sites();
    for (unsigned int i = 0 ; i < n_sites ; i += 1) {
        m_model.addConstr(t_v[i] <= m_instance.q(i) * m_x[i]);
    }
}

void flp::MasterProblem::create_constraints_demand(std::vector<std::vector<GRBVar>> &t_y, std::vector<GRBVar> &t_v,
                                                   const RobustCertificate& t_certificate) {
    const unsigned int n_sites = m_instance.n_sites();
    const unsigned int n_clients = m_instance.n_clients();

    for (unsigned int j = 0 ; j < n_clients ; j += 1) {

        GRBQuadExpr expr;

        for (unsigned int i = 0 ; i < n_sites ; i += 1) {
            expr += t_y[i][j];
        }

        m_model.addConstr(expr >= m_instance.d(j) + t_certificate.xi(j) * m_instance.d(j) * m_instance.deviation());

    }
}

void flp::MasterProblem::create_constraints_objective(std::vector<std::vector<GRBVar>> &t_y, std::vector<GRBVar> &t_v) {
    const unsigned int n_sites = m_instance.n_sites();
    const unsigned int n_clients = m_instance.n_clients();

    GRBQuadExpr expr;

    for (unsigned int i = 0 ; i < n_sites ; i += 1) {

        expr += m_instance.a() * t_v[i] + m_instance.b() * t_v[i] * t_v[i];

        for (unsigned int j = 0 ; j < n_clients ; ++j) {
            expr += m_instance.t(i,j) * t_y[i][j];
        }

    }

    m_model.addQConstr(expr - m_tau <= 0);
}

void flp::MasterProblem::create_feasibility_constraint() {
    const unsigned int n_sites = m_instance.n_sites();
    const unsigned int n_clients = m_instance.n_clients();

    GRBLinExpr expr;

    for (unsigned int i = 0 ; i < n_sites ; i += 1) {
        expr += m_instance.q(i) * m_x[i];
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
        worst_demand += demands[k];
    }

    m_model.addConstr(expr >= worst_demand);

}
