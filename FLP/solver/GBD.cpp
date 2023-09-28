//
// Created by henri on 27.09.23.
//

#include "GBD.h"

using namespace idol;

FLP::GBD::GBD(const idol::Problems::FLP::Instance &t_instance, double t_Gamma, double t_deviation)
        : Solver(t_instance, t_Gamma, t_deviation) {

}

void FLP::GBD::augment_master_problem(const idol::Solution::Primal &t_separation_solution) {

    const unsigned int n_facilities = m_instance.n_facilities();
    const unsigned int n_customers = m_instance.n_customers();

    const auto fixed_costs = idol_Sum(i, Range(n_facilities), m_instance.fixed_cost(i) * m_x[i]);

    Expr cut =

            idol_Sum(i,
                     Range(n_facilities),
                     - t_separation_solution.get(m_z[i])
                     + 2 * m_a * ( t_separation_solution.get(m_alpha[i]) - t_separation_solution.get(m_gamma[i]) )
                     - t_separation_solution.get(m_gamma[i]) * m_instance.capacity(i) * m_x[i]
            )
            -
            idol_Sum(j,
                     Range(n_customers),
                     t_separation_solution.get(m_beta[j]) * m_instance.demand(j)
                     + t_separation_solution.get(m_omega[j]) * m_instance.demand(j) * m_deviation
            )
            - t_separation_solution.get(m_lambda_0) * (t_separation_solution.get(m_x_0) + n_facilities * m_a * m_a - fixed_costs )

    ;

    m_master_problem.add_ctr(cut <= 0);

}
