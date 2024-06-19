//
// Created by henri on 18.06.24.
//

#ifndef CONVEXADJUSTALBEROBUSTOPTIMIZATION_CCG_H
#define CONVEXADJUSTALBEROBUSTOPTIMIZATION_CCG_H

#include "Solver.h"

namespace FLP {
    class CCG;
}

class FLP::CCG : public FLP::Solver {
protected:
    void augment_master_problem(const idol::Solution::Primal &t_separation_solution) override;
public:
    CCG(const FLP::Instance& t_instance, double t_Gamma, double t_deviation);
};

#endif //CONVEXADJUSTALBEROBUSTOPTIMIZATION_CCG_H
