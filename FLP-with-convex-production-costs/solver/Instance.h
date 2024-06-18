//
// Created by henri on 18.06.24.
//

#ifndef CONVEXADJUSTALBEROBUSTOPTIMIZATION_INSTANCE_H
#define CONVEXADJUSTALBEROBUSTOPTIMIZATION_INSTANCE_H

#include "idol/problems/facility-location-problem/FLP_Instance.h"

namespace FLP {
    class Instance;
}

class FLP::Instance : public idol::Problems::FLP::Instance {
    std::vector<double> m_diseconomies_of_scale;
public:
    Instance(unsigned int t_n_facilities, unsigned int t_n_customers);

    static Instance read_from_file(const std::string &t_filename);

    [[nodiscard]] double diseconomy_of_scale_factor(unsigned int t_facility) const;
    void set_diseconomy_of_scale_factor(unsigned int t_facility, double t_value);
};


#endif //CONVEXADJUSTALBEROBUSTOPTIMIZATION_INSTANCE_H
