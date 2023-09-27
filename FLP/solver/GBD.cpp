//
// Created by henri on 27.09.23.
//

#include "GBD.h"

FLP::GBD::GBD(const idol::Problems::FLP::Instance &t_instance, double t_Gamma, double t_deviation)
        : Solver(t_instance, t_Gamma, t_deviation) {

}

void FLP::GBD::augment_master_problem(const idol::Solution::Primal &t_separation_solution) {
    throw std::runtime_error("Not implemented GBD::augment_master_problem.");
}
