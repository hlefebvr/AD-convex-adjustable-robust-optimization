//
// Created by henri on 28.09.23.
//

#ifndef CONVEXADJUSTALBEROBUSTOPTIMIZATION_GBD_H
#define CONVEXADJUSTALBEROBUSTOPTIMIZATION_GBD_H

#include "Solver.h"

namespace RAP {
    class GBD;
}

class RAP::GBD : public RAP::Solver {
protected:
    void augment_master_problem(const idol::Solution::Primal &t_separation_solution) override;

public:
    GBD(const Instance& t_instance, double t_Gamma, double t_deviation, bool t_use_bilevel_separation, bool t_use_budgeted_uncertainty_set);
};


#endif //CONVEXADJUSTALBEROBUSTOPTIMIZATION_GBD_H
