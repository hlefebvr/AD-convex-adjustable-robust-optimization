//
// Created by henri on 27.09.23.
//

#include "Solver.h"
#include "optimizers/solvers/gurobi/Gurobi.h"
#include "optimizers/solvers/Mosek.h"

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
          m_xi(idol::Var::make_vector(m_env, Dim<1>(t_instance.n_customers()), 0, 1, Binary, "xi")),
          m_alpha(idol::Var::make_vector(m_env, Dim<1>(t_instance.n_facilities()), -1, 1, Continuous, "alpha")),
          m_beta(idol::Var::make_vector(m_env, Dim<1>(t_instance.n_customers()), -1, 1, Continuous, "beta")),
          m_gamma(idol::Var::make_vector(m_env, Dim<1>(t_instance.n_facilities()), 0, 1, Continuous, "gamma")),
          m_z(idol::Var::make_vector(m_env, Dim<1>(t_instance.n_facilities()), 0, Inf, Continuous, "z")),
          m_omega(idol::Var::make_vector(m_env, Dim<1>(t_instance.n_customers()), -1, 1, Continuous, "omega")),
          m_lambda_0(m_env, 0, Inf, Continuous, "lambda_0")
{

}

void FLP::Solver::initialize() {

    create_master_problem();
    create_separation_problem();

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

    const unsigned int n_facilities = m_instance.n_facilities();
    const unsigned int n_customers = m_instance.n_customers();

    const double fixed_costs = idol_Sum(i, Range(n_facilities), m_instance.fixed_cost(i) * t_separation_solution.get(m_x[i])).constant().numerical();

    Expr objective =

        idol_Sum(i,
                 Range(n_facilities),
                 - m_z[i]
                 + (m_a / (2 * m_b)) * ( m_alpha[i] - m_gamma[i] )
                 - ( m_a * m_a / ( 4 * m_b ) ) * m_lambda_0
                 + ( m_lambda_0 * m_instance.fixed_cost(i) - m_gamma[i] * m_instance.capacity(i) ) * t_separation_solution.get(m_x[i])
        )
        -
        idol_Sum(j,
                 Range(n_customers),
                 m_beta[j] * m_instance.demand(j)
                 + m_omega[j] * m_instance.demand(j) * m_deviation
        )
        - m_lambda_0 * t_separation_solution.get(m_x_0)

        ;

    m_separation_problem.set_obj_expr(objective);

    // std::cout << m_separation_problem << std::endl;

}

void FLP::Solver::create_master_problem() {

    m_master_problem.add(m_x_0);
    m_master_problem.add_vector<Var, 1>(m_x);
    m_master_problem.set_obj_expr(m_x_0);

    m_master_problem.use(Mosek() );

}

void FLP::Solver::create_separation_problem() {

    const unsigned int n_facilities = m_instance.n_facilities();
    const unsigned int n_customers = m_instance.n_customers();

    m_separation_problem.set_obj_sense(Maximize);

    m_separation_problem.add_vector<Var, 1>(m_xi);
    m_separation_problem.add_vector<Var, 1>(m_alpha);
    m_separation_problem.add_vector<Var, 1>(m_beta);
    m_separation_problem.add_vector<Var, 1>(m_gamma);
    m_separation_problem.add_vector<Var, 1>(m_omega);
    m_separation_problem.add_vector<Var, 1>(m_z);
    m_separation_problem.add(m_lambda_0);

    // LP dual constraints
    for (auto i : Range(n_facilities)) {
        for (auto j : Range(n_customers)) {
            m_separation_problem.add_ctr(m_alpha[i] + m_beta[j] + m_instance.per_unit_transportation_cost(i, j) * m_lambda_0 == 0);
        }
    }

    // Conic F dual constraints
    for (auto i : Range(n_facilities)) {
        auto auxiliary_variable = m_separation_problem.add_var(-Inf, Inf, Continuous);
        m_separation_problem.add_ctr(auxiliary_variable == m_alpha[i] - m_gamma[i] );
        m_separation_problem.add_ctr(auxiliary_variable * auxiliary_variable <= 4 * b * m_z[i] * m_lambda_0 );
    }

    // Norm constraints
    Expr sum_absolute_values = m_lambda_0 * m_lambda_0
            + idol_Sum(i, Range(n_facilities), m_gamma[i] * m_gamma[i] + m_alpha[i] * m_alpha[i])
            + idol_Sum(j, Range(n_customers), m_beta[j] * m_beta[j])
    ;
    m_separation_problem.add_ctr(sum_absolute_values <= 1);

    // Xi constraints
    m_separation_problem.add_ctr(idol_Sum(j, Range(n_customers), m_xi[j]) <= m_Gamma);

    // Linearization constraints omega_j = xi_j beta_j
    for (auto j : Range(n_customers)) {
        m_separation_problem.add_ctr(m_omega[j] == 0);
        m_separation_problem.add_ctr(m_xi[j] == 0);
        /*
        m_separation_problem.add_ctr( -m_xi[j] <= m_omega[j] );
        m_separation_problem.add_ctr( m_omega[j] <= m_xi[j] );
        m_separation_problem.add_ctr( m_beta[j] - (1 - m_xi[j]) <= m_omega[j] );
        m_separation_problem.add_ctr( m_omega[j] <= m_beta[j] + (1 - m_xi[j]) );
         */
    }

    m_separation_problem.use(
            Mosek() //.with_log_level(Info, Black)
        );

}
