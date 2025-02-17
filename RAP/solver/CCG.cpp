//
// Created by henri on 28.09.23.
//

#include "CCG.h"
#include "idol/optimizers/mixed-integer-optimization/wrappers/Mosek/Mosek.h"

using namespace idol;

RAP::CCG::CCG(const RAP::Instance &t_instance, double t_Gamma, double t_deviation)
    : Solver(t_instance, t_Gamma, t_deviation) {

}

void RAP::CCG::augment_master_problem(const idol::Solution::Primal &t_separation_solution) {

    const unsigned int n_servers = m_instance.n_servers();
    const unsigned int n_clients = m_instance.n_clients();

    auto y = m_master_problem.add_vars(Dim<2>(n_servers, n_clients), 0, Inf, Continuous);

    m_master_problem.add_ctr(
        m_x_0 >= idol_Sum(i, Range(n_servers), m_instance.unitary_cost(i) * m_x[i])
    );

    auto v = m_master_problem.add_vars(Dim<1>(n_servers), 0, Inf, Continuous);

    for (auto i : Range(n_servers)) {
        m_master_problem.add_ctr(idol_Sum(j, Range(n_clients), y[i][j]) == v[i]);
    }

    for (auto i : Range(n_servers)) {
        m_master_problem.add_ctr(v[i] + m_instance.congestion_factor(i) * v[i] * v[i] <= m_x[i]);
    }

    for (auto j : Range(n_clients)) {
        m_master_problem.add_ctr(idol_Sum(i, Range(n_servers), m_instance.service_rate(i, j) * y[i][j]) >= m_instance.demand(j) * ( 1 + m_deviation * t_separation_solution.get(m_xi[j]) ));
    }

    /*
    for (auto i : Range(n_servers)) {
        const double b_i = m_instance.congestion_factor(i);
        m_master_problem.add_ctr(
            idol_Sum(j, Range(n_clients), y[i][j] + b_i * y[i][j] * y[i][j])
            + idol_Sum(j, Range(n_clients), idol_Sum(k, Range(j+1, n_clients), 2. * b_i * y[i][j] * y[i][k]))
            <= m_x[i]
        );
    }
     */

    m_master_problem.use( Mosek() );
}
