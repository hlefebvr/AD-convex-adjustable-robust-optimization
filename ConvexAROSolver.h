//
// Created by henri on 27.09.23.
//

#ifndef CONVEXADJUSTALBEROBUSTOPTIMIZATION_CONVEXAROSOLVER_H
#define CONVEXADJUSTALBEROBUSTOPTIMIZATION_CONVEXAROSOLVER_H

#include "modeling.h"

class ConvexAROSolver {
protected:
    virtual void initialize() = 0;
    virtual idol::Solution::Primal solve_master_problem(double t_time_limit) = 0;
    virtual idol::Solution::Primal solve_separation_problem(double t_time_limit) = 0;
    virtual void update_separation_objective_function(const idol::Solution::Primal& t_separation_solution) = 0;
    virtual void augment_master_problem(const idol::Solution::Primal& t_separation_solution) = 0;
public:
    virtual ~ConvexAROSolver() = default;

    void solve(double t_time_limit, double t_tolerance_for_separation);
};


#endif //CONVEXADJUSTALBEROBUSTOPTIMIZATION_CONVEXAROSOLVER_H
