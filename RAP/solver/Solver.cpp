//
// Created by henri on 28.09.23.
//

#include "Solver.h"
#include "optimizers/solvers/Mosek.h"
#include "optimizers/solvers/gurobi/Gurobi.h"

using namespace idol;

RAP::Solver::Solver(const RAP::Instance &t_instance, double t_Gamma, double t_deviation)
    : m_instance(t_instance),
      m_Gamma(t_Gamma),
      m_deviation(t_deviation),
      m_master_problem(m_env),
      m_x_0(m_env, 0, Inf, Continuous, "x__0"),
      m_x(idol::Var::make_vector(m_env, Dim<1>(t_instance.n_servers()), 0, Inf, Continuous, "x")),
      m_separation_problem(m_env),
      m_xi(idol::Var::make_vector(m_env, Dim<1>(t_instance.n_clients()), 0, 1, Binary, "xi")),
      m_alpha(idol::Var::make_vector(m_env, Dim<1>(t_instance.n_servers()), -1, 1, Continuous, "alpha")),
      m_beta(idol::Var::make_vector(m_env, Dim<1>(t_instance.n_clients()), 0, 1, Continuous, "beta")),
      m_gamma(idol::Var::make_vector(m_env, Dim<1>(t_instance.n_servers()), 0, 1, Continuous, "gamma")),
      m_z(idol::Var::make_vector(m_env, Dim<1>(t_instance.n_servers()), 0, Inf, Continuous, "z")),
      m_omega(idol::Var::make_vector(m_env, Dim<1>(t_instance.n_clients()), 0, 1, Continuous, "omega")),
      m_lambda_0(m_env, 0, 1, Continuous, "lambda_0")
      {

}

void RAP::Solver::initialize() {

    create_master_problem();
    create_separation_problem();

}

idol::Solution::Primal RAP::Solver::solve_master_problem(double t_time_limit) {

    // std::cout << m_model << std::endl;

    m_master_problem.optimizer().set_param_time_limit(t_time_limit);
    m_master_problem.optimize();

    return save_primal(m_master_problem);
}

idol::Solution::Primal RAP::Solver::solve_separation_problem(double t_time_limit) {

    // std::cout << m_separation_problem << std::endl;

    m_separation_problem.optimizer().set_param_time_limit(t_time_limit);
    m_separation_problem.optimize();

    return save_primal(m_separation_problem);
}

void RAP::Solver::update_separation_objective_function(const Solution::Primal &t_separation_solution) {

    const unsigned int n_servers = m_instance.n_servers();
    const unsigned int n_clients = m_instance.n_clients();

    const double sum_unitary_costs = idol_Sum(i, Range(n_servers), m_instance.unitary_cost(i) * t_separation_solution.get(m_x[i]) ).constant().numerical();

    const Expr objective =

            idol_Sum(i,
                     Range(n_servers),
                     - m_z[i]
                     - 1 / (2 * m_instance.congestion_factor(i)) * m_alpha[i]
                     - 1 / (4 * m_instance.congestion_factor(i)) * m_gamma[i]
                     - m_gamma[i] * t_separation_solution.get(m_x[i])
            )
            +
            idol_Sum(j,
                     Range(n_clients),
                     m_beta[j] * m_instance.demand(j)
                     + m_omega[j] * m_instance.demand(j) * m_deviation
            )
            + m_lambda_0 * ( sum_unitary_costs - t_separation_solution.get(m_x_0) )
    ;

    m_separation_problem.set_obj_expr(objective);

}

void RAP::Solver::create_master_problem() {

    m_master_problem.add(m_x_0);
    m_master_problem.add_vector<Var, 1>(m_x);
    m_master_problem.set_obj_expr(m_x_0);

    m_master_problem.use(Mosek() );

}

void RAP::Solver::create_separation_problem() {

    const unsigned int n_servers = m_instance.n_servers();
    const unsigned int n_clients = m_instance.n_clients();

    m_separation_problem.set_obj_sense(Maximize);

    m_separation_problem.add_vector<Var, 1>(m_xi);
    m_separation_problem.add_vector<Var, 1>(m_alpha);
    m_separation_problem.add_vector<Var, 1>(m_beta);
    m_separation_problem.add_vector<Var, 1>(m_gamma);
    m_separation_problem.add_vector<Var, 1>(m_omega);
    m_separation_problem.add_vector<Var, 1>(m_z);
    m_separation_problem.add(m_lambda_0);

    // LP dual constraints
    for (auto i : Range(n_servers)) {
        for (auto j : Range(n_clients)) {
            m_separation_problem.add_ctr(m_alpha[i] - m_instance.service_rate(i, j) * m_beta[j] >= 0);
        }
    }

    // Conic F dual constraints
    for (auto i : Range(n_servers)) {
        m_separation_problem.add_ctr(m_alpha[i] * m_alpha[i] <= 4 * m_instance.congestion_factor(i) * m_z[i] * m_gamma[i] );
    }

    // Norm constraints
    Expr sum_square = m_lambda_0 * m_lambda_0
                      + idol_Sum(i, Range(n_servers), m_gamma[i] * m_gamma[i] + m_alpha[i] * m_alpha[i])
                      + idol_Sum(j, Range(n_clients), m_beta[j] * m_beta[j])
    ;
    m_separation_problem.add_ctr(sum_square <= 1);

    // Xi constraints
    m_separation_problem.add_ctr(idol_Sum(j, Range(n_clients), m_xi[j]) <= m_Gamma);

    // Linearization constraints omega_j = xi_j beta_j
    for (auto j : Range(n_clients)) {
        m_separation_problem.add_ctr( m_omega[j] <= m_xi[j] );
        m_separation_problem.add_ctr( m_omega[j] <= m_beta[j] );
        m_separation_problem.add_ctr( m_omega[j] >= m_beta[j] - (1 - m_xi[j]) );
    }

    m_separation_problem.use(
            Mosek() //.with_log_level(Info, Black)
    );

}
