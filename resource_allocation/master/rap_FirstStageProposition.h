//
// Created by henri on 28/07/22.
//

#ifndef CONVEX_ARO_RAP_FIRSTSTAGEPROPOSITION_H
#define CONVEX_ARO_RAP_FIRSTSTAGEPROPOSITION_H

#include <vector>

namespace rap {
    class FirstStageProposition;
    class Instance;
}

class rap::FirstStageProposition {
    std::vector<double> m_x_values;
public:
    explicit FirstStageProposition(const rap::Instance& t_instance);

    [[nodiscard]] double x(unsigned int t_i) const { return m_x_values[t_i]; }
    void set_x_value(unsigned int t_i, double t_v) { m_x_values[t_i] = t_v; }
};


#endif //CONVEX_ARO_RAP_FIRSTSTAGEPROPOSITION_H
