//
// Created by henri on 28.09.23.
//

#include <cassert>
#include "Solver.h"
#include "idol/optimizers/mixed-integer-optimization/wrappers/Mosek/Mosek.h"
#include "idol/optimizers/mixed-integer-optimization/wrappers/Gurobi/Gurobi.h"

using namespace idol;

RAP::Solver::Solver(const RAP::Instance &t_instance, double t_Gamma, double t_deviation, bool t_use_bilevel_separation, bool t_use_budgeted_uncertainty_set)
    : m_instance(t_instance),
      m_Gamma(t_Gamma),
      m_use_budgeted_uncertainty_set(t_use_budgeted_uncertainty_set),
      m_use_bilevel_separation(t_use_bilevel_separation),
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
      m_lambda_0(m_env, 0, 1, Continuous, "lambda_0"),

      m_dual_kp_lambda(m_env, 0, Inf, Continuous, "dual_kp_lambda"),
      m_dual_kp_mu(idol::Var::make_vector(m_env, Dim<1>(t_instance.n_clients()), 0, Inf, Continuous, "dual_kp_mu")),
      m_dual_kp_nu(idol::Var::make_vector(m_env, Dim<1>(t_instance.n_clients()), 0, Inf, Continuous, "dual_kp_nu"))
      {}

void RAP::Solver::initialize() {

    create_master_problem();
    create_separation_problem();

}

idol::Solution::Primal RAP::Solver::solve_master_problem(double t_time_limit) {

    m_master_problem.optimizer().set_param_time_limit(t_time_limit);
    m_master_problem.optimize();

    Solution::Primal result;

    if (const auto status = m_master_problem.get_status() ; status != Optimal && status != Feasible ) {
        result.set_status(status);
        result.set_reason(m_master_problem.get_reason());
        return result;
    }

    return save_primal(m_master_problem);
}

idol::Solution::Primal RAP::Solver::solve_separation_problem(double t_time_limit) {

    m_separation_problem.optimizer().set_param_time_limit(t_time_limit);
    m_separation_problem.optimize();

    Solution::Primal result;

    if (const auto status = m_separation_problem.get_status() ; status != Optimal && status != Feasible ) {
        result.set_status(status);
        result.set_reason(m_separation_problem.get_reason());
        std::cerr << "Separation problem ended with status " << status << "(" << m_separation_problem.get_reason() << ")" << std::endl;
        return result;
    }

    return save_primal(m_separation_problem);
}

void RAP::Solver::create_master_problem() {

    m_master_problem.add(m_x_0);
    m_master_problem.add_vector<Var, 1>(m_x);
    m_master_problem.set_obj_expr(m_x_0);

    m_master_problem.use(create_mosek());
}

void RAP::Solver::create_separation_problem() {

    const unsigned int n_servers = m_instance.n_servers();
    const unsigned int n_clients = m_instance.n_clients();

    m_separation_problem.set_obj_sense(Maximize);

    m_separation_problem.add_vector<Var, 1>(m_xi);
    m_separation_problem.add_vector<Var, 1>(m_alpha);
    m_separation_problem.add_vector<Var, 1>(m_beta);
    m_separation_problem.add_vector<Var, 1>(m_gamma);
    m_separation_problem.add_vector<Var, 1>(m_z);
    m_separation_problem.add(m_lambda_0);

    // LP dual constraints
    for (auto i: Range(n_servers)) {
        for (auto j: Range(n_clients)) {
            m_separation_problem.add_ctr(m_alpha[i] - m_instance.service_rate(i, j) * m_beta[j] >= 0);
        }
    }

    // Conic F dual constraints
    for (auto i: Range(n_servers)) {
        m_separation_problem.add_ctr(
                m_alpha[i] * m_alpha[i] <= 4 * m_instance.congestion_factor(i) * m_z[i] * m_gamma[i]);
    }

    // Norm constraints
    Expr sum_square = m_lambda_0 * m_lambda_0
                      + idol_Sum(i, Range(n_servers), m_gamma[i] * m_gamma[i] + m_alpha[i] * m_alpha[i])
                      + idol_Sum(j, Range(n_clients), m_beta[j] * m_beta[j]);
    m_separation_problem.add_ctr(sum_square <= 1);

    // Xi constraints
    if (m_use_budgeted_uncertainty_set) {
        m_separation_problem.add_ctr(idol_Sum(j, Range(n_clients), m_xi[j]) <= m_Gamma);
    } else {
        m_separation_problem.add_ctr(idol_Sum(j, Range(n_clients), std::ceil(m_deviation * m_instance.demand(j)) * m_xi[j]) <= Gamma_tilde());
    }

    if (m_use_bilevel_separation) {

        m_separation_problem.add(m_dual_kp_lambda);
        m_separation_problem.add_vector<Var, 1>(m_dual_kp_mu);
        m_separation_problem.add_vector<Var, 1>(m_dual_kp_nu);

        // Dual constraints
        if (m_use_budgeted_uncertainty_set) {
            for (unsigned int j = 0 ; j < n_clients ; ++j) {
                m_separation_problem.add_ctr(m_deviation * m_instance.demand(j) * m_beta[j] - m_dual_kp_lambda - m_dual_kp_mu[j] + m_dual_kp_nu[j] == 0);
            }
        } else {

            for (unsigned int j = 0 ; j < n_clients ; ++j) {
                m_separation_problem.set_var_type(m_xi[j], Continuous);
                m_separation_problem.set_var_ub(m_xi[j], 1);
                m_separation_problem.set_var_lb(m_xi[j], 0);
            }

            for (unsigned int j = 0 ; j < n_clients ; ++j) {
                m_separation_problem.add_ctr(m_deviation * m_instance.demand(j) * m_beta[j] - std::ceil(m_deviation * m_instance.demand(j)) * m_dual_kp_lambda - m_dual_kp_mu[j] + m_dual_kp_nu[j] == 0);
            }
        }

        const auto add_linearization = [&](const Expr<>& t_a, const Expr<>& t_b, const Var& t_bin_var, double t_big_M) {
            t_big_M = std::ceil(t_big_M);
            m_separation_problem.add_ctr(t_a <= t_big_M * t_bin_var);
            m_separation_problem.add_ctr(t_b <= t_big_M * (1 - t_bin_var));
        };

        // Complementarity constraints
        if (m_use_budgeted_uncertainty_set) {
            /// Knapsack
            const auto z_1 = m_separation_problem.add_var(0, 1, Binary, "z_1");
            add_linearization(m_dual_kp_lambda, m_Gamma - idol_Sum(j, Range(n_clients), m_xi[j]), z_1, compute_max_demand());
            /// Bounds
            for (unsigned int j = 0 ; j < n_clients ; ++j) {
                const auto z_2 = m_separation_problem.add_var(0, 1, Binary, "z_2_" + std::to_string(j));
                add_linearization(m_dual_kp_mu[j], 1 - m_xi[j], z_2, m_instance.demand(j)); // UB

                const auto z_3 = m_separation_problem.add_var(0, 1, Binary, "z_3_" + std::to_string(j));
                add_linearization(m_dual_kp_nu[j], m_xi[j], z_3, compute_max_demand() + m_instance.demand(j)); // LB
            }
        } else {
            /// Knapsack
            const auto z_1 = m_separation_problem.add_var(0, 1, Binary, "z_1");
            const double sum_deviation_per_Gamma_tilde = idol_Sum(j, Range(n_clients), m_deviation * m_instance.demand(j)).constant().as_numerical() / Gamma_tilde();
            add_linearization(m_dual_kp_lambda, Gamma_tilde() - idol_Sum(j, Range(n_clients), std::ceil(m_deviation * m_instance.demand(j)) * m_xi[j]), z_1, std::max(sum_deviation_per_Gamma_tilde, Gamma_tilde()));
            /// Bounds
            for (unsigned int j = 0 ; j < n_clients ; ++j) {
                const auto z_2 = m_separation_problem.add_var(0, 1, Binary, "z_2_" + std::to_string(j));
                add_linearization(m_dual_kp_mu[j], 1 - m_xi[j], z_2, std::max(1., m_deviation * m_instance.demand(j))); // UB

                const auto z_3 = m_separation_problem.add_var(0, 1, Binary, "z_3_" + std::to_string(j));
                add_linearization(m_dual_kp_nu[j], m_xi[j], z_3, std::max(1., 2 * m_deviation * m_instance.demand(j) + sum_deviation_per_Gamma_tilde)); // LB
            }
        }

        // Strong-duality cut
        const auto s_1 = m_separation_problem.add_vars(Dim<1>(n_clients), 0, 1, Continuous, "s_1");
        const auto s_2 = m_separation_problem.add_vars(Dim<1>(n_clients), 0, 1, Continuous, "s_2");
        for (unsigned int j = 0 ; j < n_clients ; ++j) {
            m_separation_problem.add_ctr(m_beta[j] == s_1[j] - s_2[j]);
        }
        m_separation_problem.add_ctr(idol_Sum(j, Range(n_clients), (s_1[j] + s_2[j]) * m_instance.demand(j) * m_deviation)
                    >= (m_use_budgeted_uncertainty_set ? m_Gamma : Gamma_tilde()) * m_dual_kp_lambda + idol_Sum(j, Range(n_clients), m_beta[j] * m_instance.demand(j) * m_deviation + m_dual_kp_mu[j]));

    } else {

        assert(m_use_budgeted_uncertainty_set);

        m_separation_problem.add_vector<Var, 1>(m_omega);

        // Xi constraints
        m_separation_problem.add_ctr(idol_Sum(j, Range(n_clients), m_xi[j]) <= m_Gamma);

        // Linearization constraints omega_j = xi_j beta_j
        for (auto j: Range(n_clients)) {
            m_separation_problem.add_ctr(m_omega[j] <= m_xi[j]);
            m_separation_problem.add_ctr(m_omega[j] <= m_beta[j]);
            m_separation_problem.add_ctr(m_omega[j] >= m_beta[j] - (1 - m_xi[j]));
        }

    }

    m_separation_problem.use(create_mosek().add_callback(EarlyStopCallback(*this)));

}

void RAP::Solver::update_separation_objective_function(const Solution::Primal &t_master_solution) {

    const unsigned int n_servers = m_instance.n_servers();
    const unsigned int n_clients = m_instance.n_clients();

    const double sum_unitary_costs = idol_Sum(i, Range(n_servers), m_instance.unitary_cost(i) * t_master_solution.get(m_x[i]) ).constant().numerical();

    if (m_use_bilevel_separation) {

        const Expr objective =

                idol_Sum(i,
                         Range(n_servers),
                         - m_z[i]
                         + 1 / (2 * m_instance.congestion_factor(i)) * m_alpha[i]
                         - 1 / (4 * m_instance.congestion_factor(i)) * m_gamma[i]
                         - m_gamma[i] * t_master_solution.get(m_x[i])
                )
                +
                (m_use_budgeted_uncertainty_set ? m_Gamma : Gamma_tilde()) * m_dual_kp_lambda
                +
                idol_Sum(j,
                         Range(n_clients),
                         m_beta[j] * m_instance.demand(j)
                         + m_dual_kp_mu[j]
                )
                + m_lambda_0 * (sum_unitary_costs - t_master_solution.get(m_x_0) )
        ;

        m_separation_problem.set_obj_expr(objective);

    } else {

        const Expr objective =

                idol_Sum(i,
                         Range(n_servers),
                         - m_z[i]
                         + 1 / (2 * m_instance.congestion_factor(i)) * m_alpha[i]
                         - 1 / (4 * m_instance.congestion_factor(i)) * m_gamma[i]
                         - m_gamma[i] * t_master_solution.get(m_x[i])
                )
                +
                idol_Sum(j,
                         Range(n_clients),
                         m_beta[j] * m_instance.demand(j)
                         + m_omega[j] * m_instance.demand(j) * m_deviation
                )
                + m_lambda_0 * (sum_unitary_costs - t_master_solution.get(m_x_0) )
        ;

        m_separation_problem.set_obj_expr(objective);

    }

}

double RAP::Solver::Gamma_tilde() const {
    const double n_clients = m_instance.n_clients();
    const double sum_demands = m_deviation * idol_Sum(j, Range(n_clients), m_instance.demand(j)).constant().numerical();
    const double result = std::ceil( m_Gamma * sum_demands / n_clients);
    return result;
}

double RAP::Solver::compute_max_demand() const {
    double result = 0;
    const double n_clients = m_instance.n_clients();
    for (unsigned int j = 0 ; j < n_clients ; ++j) {
        result = std::max(result, m_instance.demand(j));
    }
    return result;
}

idol::Mosek RAP::Solver::create_mosek() const {

    auto mosek = Mosek();
    //mosek.with_external_parameter("intpntCoTolPfeas", 1e-6);
    //mosek.with_external_parameter("intpntCoTolRelGap", 1e-6);
    //mosek.with_external_parameter("intpntCoTolMuRed", 1e-6);
    mosek.with_logs(false);

    return std::move(mosek);
}
