//
// Created by henri on 28.09.23.
//

#include "GBD.h"
#include "idol/optimizers/mixed-integer-optimization/wrappers/Mosek/Mosek.h"

using namespace idol;

RAP::GBD::GBD(const RAP::Instance &t_instance, double t_Gamma, double t_deviation, bool t_use_bilevel_separation, bool t_use_budgeted_uncertainty_set)
    : Solver(t_instance, t_Gamma, t_deviation, t_use_bilevel_separation, t_use_budgeted_uncertainty_set) {

}

void RAP::GBD::augment_master_problem(const idol::Solution::Primal &t_separation_solution) {

    const unsigned int n_servers = m_instance.n_servers();
    const unsigned int n_clients = m_instance.n_clients();

    const Expr sum_unitary_costs = idol_Sum(i, Range(n_servers), m_instance.unitary_cost(i) * m_x[i] );

    const Expr cut =

            idol_Sum(i,
                     Range(n_servers),
                     - t_separation_solution.get(m_z[i])
                     + 1 / (2 * m_instance.congestion_factor(i)) * t_separation_solution.get(m_alpha[i])
                     - 1 / (4 * m_instance.congestion_factor(i)) * t_separation_solution.get(m_gamma[i])
                     - t_separation_solution.get(m_gamma[i]) * m_x[i]
            )
            +
            idol_Sum(j,
                     Range(n_clients),
                     t_separation_solution.get(m_beta[j]) * m_instance.demand(j)
                     + t_separation_solution.get(m_omega[j]) * m_instance.demand(j) * m_deviation
            )
            + t_separation_solution.get(m_lambda_0) * ( sum_unitary_costs - m_x_0 )
    ;

    m_master_problem.add_ctr(cut <= 0);

}
