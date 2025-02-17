//
// Created by henri on 27.09.23.
//

#ifndef CONVEXADJUSTALBEROBUSTOPTIMIZATION_CONVEXAROSOLVER_H
#define CONVEXADJUSTALBEROBUSTOPTIMIZATION_CONVEXAROSOLVER_H

#include "idol/modeling.h"
#include "idol/optimizers/mixed-integer-optimization/callbacks/CallbackFactory.h"
#include "idol/optimizers/mixed-integer-optimization/callbacks/Callback.h"

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

    virtual Report solve(double t_time_limit, double t_tolerance_for_separation, bool t_heuristic_mode) = 0;
};

class ConvexAROSolver : public AbstractSolver {
    bool m_heuristic_mode = false;
protected:
    virtual void initialize() = 0;
    virtual idol::Solution::Primal solve_master_problem(double t_time_limit) = 0;
    virtual idol::Solution::Primal solve_separation_problem(double t_time_limit) = 0;
    virtual void update_separation_objective_function(const idol::Solution::Primal& t_master_solution) = 0;
    virtual void augment_master_problem(const idol::Solution::Primal& t_separation_solution) = 0;
public:
    Report solve(double t_time_limit, double t_tolerance_for_separation, bool t_heuristic_mode) override;

    [[nodiscard]] bool heuristic_mode() const { return m_heuristic_mode; }
};

class EarlyStopCallback : public idol::CallbackFactory {
    const ConvexAROSolver* m_parent;
public:
    explicit EarlyStopCallback(const ConvexAROSolver& t_parent) : m_parent(&t_parent) {}

    EarlyStopCallback(const EarlyStopCallback&) = default;
    EarlyStopCallback(EarlyStopCallback&&) = default;

    EarlyStopCallback& operator=(const EarlyStopCallback&) = default;
    EarlyStopCallback& operator=(EarlyStopCallback&&) = default;

    class Strategy : public idol::Callback {
        const ConvexAROSolver* m_parent;
    protected:
        void operator()(idol::CallbackEvent t_event) override;
    public:
        explicit Strategy(const ConvexAROSolver& t_parent);
    };

    idol::Callback *operator()() override {
        return new Strategy(*m_parent);
    }

    [[nodiscard]] idol::CallbackFactory *clone() const override {
        return new EarlyStopCallback(*this);
    }
};

#endif //CONVEXADJUSTALBEROBUSTOPTIMIZATION_CONVEXAROSOLVER_H
