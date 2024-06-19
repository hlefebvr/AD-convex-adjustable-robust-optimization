//
// Created by henri on 28.09.23.
//

#ifndef CONVEXADJUSTALBEROBUSTOPTIMIZATION_NOMINAL_H
#define CONVEXADJUSTALBEROBUSTOPTIMIZATION_NOMINAL_H

#include "../../ConvexAROSolver.h"
#include "idol/problems/facility-location-problem/FLP_Instance.h"

namespace FLP {
    class Nominal;
}

class FLP::Nominal : public AbstractSolver {
    const idol::Problems::FLP::Instance& m_instance;
    const double m_a = .75;
    const double m_b = .75;
    idol::Env m_env;
    idol::Model m_model;
public:
    explicit Nominal(const idol::Problems::FLP::Instance& t_instance);

    Report solve(double t_time_limit, double t_tolerance_for_separation, bool t_use_heuristic) override;
};
#endif //CONVEXADJUSTALBEROBUSTOPTIMIZATION_NOMINAL_H
