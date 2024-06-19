//
// Created by henri on 18.06.24.
//

#ifndef CONVEXADJUSTALBEROBUSTOPTIMIZATION_GBD_H
#define CONVEXADJUSTALBEROBUSTOPTIMIZATION_GBD_H

#include "Solver.h"

namespace FLP {
    class GBD;

}

class FLP::GBD : public FLP::Solver {
protected:
    void augment_master_problem(const idol::Solution::Primal &t_separation_solution) override;

public:
    GBD(const FLP::Instance& t_instance, double t_Gamma, double t_deviation);
};

#endif //CONVEXADJUSTALBEROBUSTOPTIMIZATION_GBD_H
