//
// Created by henri on 18.06.24.
//

#include "CCG.h"

using namespace idol;

FLP::CCG::CCG(const FLP::Instance &t_instance, double t_Gamma, double t_deviation)
        : Solver(t_instance, t_Gamma, t_deviation) {

}

void FLP::CCG::augment_master_problem(const idol::Solution::Primal &t_separation_solution) {

    const unsigned int n_facilities = m_instance.n_facilities();
    const unsigned int n_customers = m_instance.n_customers();

    auto y = m_master_problem.add_vars(Dim<2>(n_facilities, n_customers), 0, Inf, Continuous);
    auto v = m_master_problem.add_vars(Dim<1>(n_facilities), 0, Inf, Continuous);
    auto theta = m_master_problem.add_vars(Dim<1>(n_facilities), 0, Inf, Continuous);
    auto s = m_master_problem.add_vars(Dim<1>(n_facilities), 0, Inf, Continuous);

    // Objective
    m_master_problem.add_ctr(m_x_0 >=
                    idol_Sum(
                            i,
                            Range(n_facilities),
                            m_instance.fixed_cost(i) * m_x[i]
                            + theta[i]
                            - m_instance.diseconomy_of_scale_factor(i)
                            + idol_Sum(
                                    j,
                                    Range(n_customers),
                                    m_instance.per_unit_transportation_cost(i, j) * y[i][j]
                            )
                    ),
                    "epigraph"
    );

    for (auto i : Range(n_facilities)) {
        m_master_problem.add_ctr(s[i] == m_instance.capacity(i) - v[i] + m_epsilon);
        m_master_problem.add_ctr(theta[i] * s[i]>= m_instance.diseconomy_of_scale_factor(i) * (m_instance.capacity(i) + m_epsilon));
    }

    for (auto i : Range(n_facilities)) {
        m_master_problem.add_ctr(idol_Sum(j, Range(n_customers), y[i][j]) == v[i]);
    }

    for (auto j : Range(n_customers)) {
        m_master_problem.add_ctr(idol_Sum(i, Range(n_facilities), y[i][j]) == m_instance.demand(j) * (1 + m_deviation * t_separation_solution.get(m_xi[j]) ) );
    }

    for (auto i : Range(n_facilities)) {
        m_master_problem.add_ctr(v[i] <= m_instance.capacity(i) * m_x[i]);
    }

}
