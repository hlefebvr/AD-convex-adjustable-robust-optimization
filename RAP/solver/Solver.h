//
// Created by henri on 28.09.23.
//

#ifndef CONVEXADJUSTALBEROBUSTOPTIMIZATION_SOLVER_H
#define CONVEXADJUSTALBEROBUSTOPTIMIZATION_SOLVER_H

#include "../../ConvexAROSolver.h"
#include "../instance/Instance.h"

namespace RAP {
    class Solver;
}

class RAP::Solver : public ConvexAROSolver {
protected:
    const Instance& m_instance;
    const double m_Gamma;
    const double m_deviation;

    idol::Env m_env;

    // Master problem
    idol::Model m_master_problem;
    idol::Var m_x_0;
    idol::Vector<idol::Var, 1> m_x;

    // Separation
    idol::Model m_separation_problem;
    idol::Vector<idol::Var, 1> m_xi;
    idol::Vector<idol::Var, 1> m_alpha;
    idol::Vector<idol::Var, 1> m_beta;
    idol::Vector<idol::Var, 1> m_gamma;
    idol::Vector<idol::Var, 1> m_z;
    idol::Vector<idol::Var, 1> m_omega;
    idol::Var m_lambda_0;

    void initialize() override;
    void create_master_problem();
    void create_separation_problem();

    idol::Solution::Primal solve_master_problem(double t_time_limit) override;
    idol::Solution::Primal solve_separation_problem(double t_time_limit) override;
    void update_separation_objective_function(const idol::Solution::Primal &t_master_solution) override;
public:
    Solver(const Instance& t_instance, double t_Gamma, double t_deviation);
};


#endif //CONVEXADJUSTALBEROBUSTOPTIMIZATION_SOLVER_H
