//
// Created by henri on 27.09.23.
//

#include "Solver.h"
#include "optimizers/solvers/gurobi/Gurobi.h"

using namespace idol;

FLP::Solver::Solver(const idol::Problems::FLP::Instance &t_instance,
              double t_Gamma,
              double t_deviation)
        : m_instance(t_instance),
          m_Gamma(t_Gamma),
          m_deviation(t_deviation),
          m_master_problem(m_env),
          m_x_0(m_env, 0, Inf, Continuous, "x_0"),
          m_x(idol::Var::make_vector(m_env, Dim<1>(t_instance.n_facilities()), 0, 1, Binary, "x")),
          m_separation_problem(m_env),
          m_xi(idol::Var::make_vector(m_env, Dim<1>(t_instance.n_customers()), 0, 1, Continuous, "xi"))
{

}

void FLP::Solver::initialize() {

    const unsigned int n_facilities = m_instance.n_facilities();

    // Master problem
    m_master_problem.add(m_x_0);
    m_master_problem.add_vector<Var, 1>(m_x);
    m_master_problem.set_obj_expr(idol_Sum(i, Range(n_facilities), m_instance.fixed_cost(i) * m_x[i]) + m_x_0);

    // Separation problem
    m_separation_problem.add_vector<Var, 1>(m_xi);

    // Solvers
    m_master_problem.use(Gurobi());
    m_separation_problem.use(Gurobi());

}

idol::Solution::Primal FLP::Solver::solve_master_problem(double t_time_limit) {

    m_master_problem.optimizer().set_param_time_limit(t_time_limit);
    m_master_problem.optimize();

    return save_primal(m_master_problem);
}

idol::Solution::Primal
FLP::Solver::solve_separation_problem(double t_time_limit) {

    m_separation_problem.optimizer().set_param_time_limit(t_time_limit);
    m_separation_problem.optimize();

    return save_primal(m_separation_problem);
}

void FLP::Solver::update_separation_objective_function(const Solution::Primal &t_separation_solution) {
    throw std::runtime_error("Not implemented CCG::update_separation_objective_function");
}
