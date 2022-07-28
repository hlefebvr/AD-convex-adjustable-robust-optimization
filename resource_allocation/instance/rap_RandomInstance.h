//
// Created by henri on 28/07/22.
//

#ifndef CONVEX_ARO_RAP_RANDOMINSTANCE_H
#define CONVEX_ARO_RAP_RANDOMINSTANCE_H

#include "rap_Instance.h"
#include <random>

namespace rap {
    class RandomInstance;
}

class rap::RandomInstance : public rap::Instance {
    std::random_device m_rd;
    std::mt19937 m_engine;

    void compute_service_rates();
    void compute_costs();
    void compute_alphas();
    void compute_demands();
public:
    RandomInstance(unsigned int t_n_resources, unsigned int t_n_clients);
};


#endif //CONVEX_ARO_RAP_RANDOMINSTANCE_H
