//
// Created by henri on 09/07/22.
//

#ifndef CONVEX_ARO_FLP_SEPARATIONPROBLEM_H
#define CONVEX_ARO_FLP_SEPARATIONPROBLEM_H

#include <gurobi_c++.h>

namespace flp {
    class SeparationProblem;
    class Instance;
    class FirstStageProposition;
}

class flp::SeparationProblem {
    const Instance& m_instance;
    GRBEnv m_env;
    GRBModel m_model;
    std::vector<std::vector<GRBVar>> m_pi;
    std::vector<GRBVar> m_theta;
    std::vector<GRBVar> m_v;
    std::vector<GRBVar> m_xi;
    std::vector<GRBVar> m_z;
    GRBVar m_mu;

    void create_variables(std::vector<GRBVar>& t_dest, unsigned int t_n, double t_lb, double t_ub, char t_type);
    void create_variables_pi();
    void create_variables_theta();
    void create_variables_v();
    void create_variables_z();
    void create_variables_xi();
    void create_variable_mu();
    void create_constraint_simplex();
    void create_constraints_perspective_conjugate();
    void create_constraints_farkas();
    void create_objective_without_x(double t_max_deviation);
    void create_constraint_budget(double t_gamma);
    void create_linearization_constraints();
public:
    SeparationProblem(const Instance& t_instance, double t_gamma, double t_max_deviation);
    void solve();
    void export_model(const std::string& t_model);
    void update(const FirstStageProposition& t_proposition);
};


#endif //CONVEX_ARO_FLP_SEPARATIONPROBLEM_H
