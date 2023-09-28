//
// Created by henri on 27.09.23.
//

#include "CCG.h"

using namespace idol;

FLP::CCG::CCG(const idol::Problems::FLP::Instance &t_instance, double t_Gamma, double t_deviation)
        : Solver(t_instance, t_Gamma, t_deviation) {

}

void FLP::CCG::augment_master_problem(const idol::Solution::Primal &t_separation_solution) {

    const unsigned int n_facilities = m_instance.n_facilities();
    const unsigned int n_customers = m_instance.n_customers();

    auto y = m_master_problem.add_vars(Dim<2>(n_facilities, n_customers), 0, Inf, Continuous);
    auto v = m_master_problem.add_vars(Dim<1>(n_facilities), 0, Inf, Continuous);

    // Objective
    m_master_problem.add_ctr(m_x_0 >=
                    idol_Sum(
                            i,
                            Range(n_facilities),
                            m_instance.fixed_cost(i) * m_x[i]
                            + m_a * v[i]
                            + m_b * v[i] * v[i]
                            + idol_Sum(
                                    j,
                                    Range(n_customers),
                                    m_instance.per_unit_transportation_cost(i, j) * y[i][j]
                            )
                    )
    );

    for (auto i : Range(n_facilities)) {
        m_master_problem.add_ctr(idol_Sum(j, Range(n_customers), y[i][j]) == v[i]);
    }

    for (auto j : Range(n_customers)) {
        m_master_problem.add_ctr(idol_Sum(i, Range(n_facilities), y[i][j]) == m_instance.demand(j));
    }

    for (auto i : Range(n_facilities)) {
        m_master_problem.add_ctr(v[i] <= m_instance.capacity(i) * m_x[i]);
    }

}
