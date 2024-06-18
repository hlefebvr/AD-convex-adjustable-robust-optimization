//
// Created by henri on 18.06.24.
//

#include <fstream>
#include "Instance.h"
#include "idol/errors/Exception.h"

FLP::Instance::Instance(unsigned int t_n_facilities, unsigned int t_n_customers)
    : idol::Problems::FLP::Instance(t_n_facilities, t_n_customers),
      m_diseconomies_of_scale(t_n_facilities)
    {

}

FLP::Instance FLP::Instance::read_from_file(const std::string &t_filename) {

    std::ifstream file(t_filename);

    if (!file.is_open()) {
        throw idol::Exception("Could not open instance file.");
    }

    unsigned int n_facilities, n_customers;

    file >> n_facilities >> n_customers;

    Instance result(n_facilities, n_customers);

    double placeholder;

    for (unsigned int i = 0 ; i < n_facilities ; ++i) {
        file >> placeholder;
        result.set_fixed_cost(i, placeholder);
        file >> placeholder;
        result.set_capacity(i, placeholder);
    }

    for (unsigned int j = 0 ; j < n_customers ; ++j) {
        file >> placeholder;
        result.set_demand(j, placeholder);
    }

    for (unsigned int i = 0 ; i < n_facilities ; ++i) {
        for (unsigned int j = 0 ; j < n_customers ; ++j) {
            file >> placeholder;
            result.set_per_unit_transportation_cost(i, j, placeholder);
        }
    }

    for (unsigned int i = 0 ; i < n_facilities ; ++i) {
        file >> placeholder;
        result.set_diseconomy_of_scale_factor(i, placeholder);
    }

    return result;
}

void FLP::Instance::set_diseconomy_of_scale_factor(unsigned int t_facility, double t_value) {
    m_diseconomies_of_scale[t_facility] = t_value;
}

double FLP::Instance::diseconomy_of_scale_factor(unsigned int t_facility) const {
    return m_diseconomies_of_scale[t_facility];
}
