//
// Created by henri on 28.09.23.
//

#include "Nominal.h"
#include "idol/optimizers/solvers/Mosek.h"

using namespace idol;

RAP::Nominal::Nominal(const RAP::Instance &t_instance) : m_instance(t_instance), m_model(m_env) {


    const unsigned int n_servers = m_instance.n_servers();
    const unsigned int n_clients = m_instance.n_clients();

    auto objective = m_model.add_var(-Inf, Inf, Continuous, "objective");
    auto x = m_model.add_vars(Dim<1>(n_servers), -Inf, Inf, Continuous);
    auto y = m_model.add_vars(Dim<2>(n_servers, n_clients), 0, Inf, Continuous);
    auto v = m_model.add_vars(Dim<1>(n_servers), -Inf, Inf, Continuous);

    m_model.set_obj_expr(objective);

    m_model.add_ctr(
            objective >= idol_Sum(i, Range(n_servers), m_instance.unitary_cost(i) * x[i])
    );

    for (auto i : Range(n_servers)) {
        m_model.add_ctr(idol_Sum(j, Range(n_clients), y[i][j]) == v[i]);
    }

    for (auto j : Range(n_clients)) {
        m_model.add_ctr(idol_Sum(i, Range(n_servers), m_instance.service_rate(i, j) * y[i][j]) >= m_instance.demand(j));
    }

    for (auto i : Range(n_servers)) {
        m_model.add_ctr(v[i] + m_instance.congestion_factor(i) * v[i] * v[i] <= x[i]);
    }

    m_model.use( Mosek() );

}

AbstractSolver::Report RAP::Nominal::solve(double t_time_limit, double t_tolerance_for_separation) {

    m_model.optimizer().set_param_time_limit(t_time_limit);
    m_model.optimize();

    const auto solution = save_primal(m_model);

    return {
            solution.has_objective_value() ? solution.objective_value() : Inf,
            m_model.optimizer().time().count(),
            0,
            0,
            0,
            solution.status() != Optimal && solution.reason() != TimeLimit
    };

}
