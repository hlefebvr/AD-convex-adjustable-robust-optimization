//
// Created by henri on 18.06.24.
//

#ifndef CONVEXADJUSTALBEROBUSTOPTIMIZATION_NOMINAL_H
#define CONVEXADJUSTALBEROBUSTOPTIMIZATION_NOMINAL_H

#include "../../ConvexAROSolver.h"
#include "Instance.h"

namespace FLP {
    class Nominal;
}

class FLP::Nominal : public AbstractSolver {
    const FLP::Instance& m_instance;
    idol::Env m_env;
    idol::Model m_model;
    double m_deviation;
public:
    explicit Nominal(const FLP::Instance& t_instance, double t_deviation = 0.);

    Report solve(double t_time_limit, double t_tolerance_for_separation, bool t_use_heuristc) override;
};


#endif //CONVEXADJUSTALBEROBUSTOPTIMIZATION_NOMINAL_H
