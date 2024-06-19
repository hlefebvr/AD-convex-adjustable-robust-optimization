//
// Created by henri on 18.06.24.
//

#include "GBD.h"

using namespace idol;

FLP::GBD::GBD(const FLP::Instance &t_instance, double t_Gamma, double t_deviation)
        : Solver(t_instance, t_Gamma, t_deviation) {

}

void FLP::GBD::augment_master_problem(const idol::Solution::Primal &t_separation_solution) {

    const unsigned int n_facilities = m_instance.n_facilities();
    const unsigned int n_customers = m_instance.n_customers();

    Expr cut =

            idol_Sum(i,
                     Range(n_facilities),
                     + 2 * std::sqrt( m_instance.diseconomy_of_scale_factor(i) * (m_instance.capacity(i) + m_epsilon) ) * t_separation_solution.get( m_z[i] )
                     + (m_instance.capacity(i) + m_epsilon) * ( t_separation_solution.get(m_gamma[i]) - t_separation_solution.get(m_alpha[i]))
                     - m_instance.diseconomy_of_scale_factor(i) * t_separation_solution.get(m_lambda_0)
                     + ( t_separation_solution.get(m_lambda_0) * m_instance.fixed_cost(i) - t_separation_solution.get(m_gamma[i]) * m_instance.capacity(i) ) * m_x[i]
            )
            -
            idol_Sum(j,
                     Range(n_customers),
                     t_separation_solution.get(m_beta[j]) * m_instance.demand(j)
                     + t_separation_solution.get(m_omega[j]) * m_instance.demand(j) * m_deviation
            )
            - t_separation_solution.get(m_lambda_0) * m_x_0

    ;

    m_master_problem.add_ctr(cut <= 0);


}
