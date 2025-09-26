//
// Created by henri on 28.09.23.
//

#ifndef CONVEXADJUSTALBEROBUSTOPTIMIZATION_NOMINAL_H
#define CONVEXADJUSTALBEROBUSTOPTIMIZATION_NOMINAL_H

#include "../../ConvexAROSolver.h"
#include "../instance/Instance.h"

namespace RAP {
    class Nominal;
}

class RAP::Nominal : public AbstractSolver {
    const Instance& m_instance;
    idol::Env m_env;
    idol::Model m_model;
    double m_deviation;
public:
    explicit Nominal(const Instance& t_instance, double t_deviation = 0.);

    Report solve(double t_time_limit, double t_tolerance_for_separation, bool t_use_heuristic) override;
};


#endif //CONVEXADJUSTALBEROBUSTOPTIMIZATION_NOMINAL_H
