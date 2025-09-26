//
// Created by henri on 18.06.24.
//

#include "Nominal.h"
#include "idol/optimizers/mixed-integer-optimization/wrappers/Mosek/Mosek.h"

using namespace idol;

FLP::Nominal::Nominal(const FLP::Instance &t_instance, double t_deviation)
        : m_instance(t_instance),
          m_model(m_env),
          m_deviation(t_deviation) {

    const unsigned int n_facilities = m_instance.n_facilities();
    const unsigned int n_customers = m_instance.n_customers();

    const double epsilon = 1e-3;

    auto objective = m_model.add_var(-Inf, Inf, Continuous, "objective");
    auto x = m_model.add_vars(Dim<1>(n_facilities), 0, 1, Binary, "x");
    auto y = m_model.add_vars(Dim<2>(n_facilities, n_customers), 0, Inf, Continuous, "y");
    auto v = m_model.add_vars(Dim<1>(n_facilities), 0, Inf, Continuous, "v");
    auto theta = m_model.add_vars(Dim<1>(n_facilities), 0, Inf, Continuous, "theta");
    auto s = m_model.add_vars(Dim<1>(n_facilities), 0, Inf, Continuous, "s");

    m_model.set_obj_expr(objective);

    // Objective
    m_model.add_ctr(objective >=
                    idol_Sum(
                            i,
                            Range(n_facilities),
                            m_instance.fixed_cost(i) * x[i]
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
        m_model.add_ctr(s[i] == m_instance.capacity(i) - v[i] + epsilon);
        m_model.add_ctr(theta[i] * s[i]>= m_instance.diseconomy_of_scale_factor(i) * (m_instance.capacity(i) + epsilon));
    }

    for (auto i : Range(n_facilities)) {
        m_model.add_ctr(idol_Sum(j, Range(n_customers), y[i][j]) == v[i]);
    }

    for (auto j : Range(n_customers)) {
        m_model.add_ctr(idol_Sum(i, Range(n_facilities), y[i][j]) == m_instance.demand(j) * (1. + m_deviation));
    }

    for (auto i : Range(n_facilities)) {
        m_model.add_ctr(v[i] <= m_instance.capacity(i) * x[i]);
    }

    m_model.use(Mosek());

}

AbstractSolver::Report FLP::Nominal::solve(double t_time_limit, double t_tolerance_for_separation, bool t_use_heuristc) {

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
