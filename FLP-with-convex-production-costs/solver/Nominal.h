//
// Created by henri on 18.06.24.
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
    idol::Env m_env;
    idol::Model m_model;
public:
    explicit Nominal(const idol::Problems::FLP::Instance& t_instance);

    Report solve(double t_time_limit, double t_tolerance_for_separation) override;
};


#endif //CONVEXADJUSTALBEROBUSTOPTIMIZATION_NOMINAL_H
