//
// Created by henri on 27.09.23.
//

#ifndef CONVEXADJUSTALBEROBUSTOPTIMIZATION_SOLVER_H
#define CONVEXADJUSTALBEROBUSTOPTIMIZATION_SOLVER_H

#include "../../ConvexAROSolver.h"
#include "problems/facility-location-problem/FLP_Instance.h"

namespace FLP {
    class Solver;
}

class FLP::Solver : public ConvexAROSolver {
protected:
    const idol::Problems::FLP::Instance& m_instance;
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


    void initialize() override;
    idol::Solution::Primal solve_master_problem(double t_time_limit) override;
    idol::Solution::Primal solve_separation_problem(double t_time_limit) override;
    void update_separation_objective_function(const idol::Solution::Primal &t_separation_solution) override;
public:
    Solver(const idol::Problems::FLP::Instance& t_instance, double t_Gamma, double t_deviation);
};

#endif //CONVEXADJUSTALBEROBUSTOPTIMIZATION_SOLVER_H
