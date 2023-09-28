//
// Created by henri on 27.09.23.
//

#ifndef CONVEXADJUSTALBEROBUSTOPTIMIZATION_CONVEXAROSOLVER_H
#define CONVEXADJUSTALBEROBUSTOPTIMIZATION_CONVEXAROSOLVER_H

#include "modeling.h"

class AbstractSolver {
public:
    virtual ~AbstractSolver() = default;

    struct Report {
        const double best_bound;
        const double total_time;
        const double master_time;
        const double separation_time;
        const unsigned int iteration_count;
        const bool fail_flag;

        Report(double t_best_bound, double t_total_time, double t_master_time, double t_separation_time, unsigned int t_iteration_count, bool t_fail_flag);
    };

    virtual Report solve(double t_time_limit, double t_tolerance_for_separation) = 0;
};

class ConvexAROSolver : public AbstractSolver {
protected:
    virtual void initialize() = 0;
    virtual idol::Solution::Primal solve_master_problem(double t_time_limit) = 0;
    virtual idol::Solution::Primal solve_separation_problem(double t_time_limit) = 0;
    virtual void update_separation_objective_function(const idol::Solution::Primal& t_master_solution) = 0;
    virtual void augment_master_problem(const idol::Solution::Primal& t_separation_solution) = 0;
public:
    Report solve(double t_time_limit, double t_tolerance_for_separation) override;
};

#endif //CONVEXADJUSTALBEROBUSTOPTIMIZATION_CONVEXAROSOLVER_H
