//
// Created by henri on 09/07/22.
//

#ifndef CONVEX_ARO_FLP_MASTERPROBLEM_H
#define CONVEX_ARO_FLP_MASTERPROBLEM_H

#include <memory>
#include "../../Model.h"

namespace flp {
    class MasterProblem;
    class Callback;
    class Instance;
}

class flp::MasterProblem : public Model {
    const Instance& m_instance;
    std::vector<GRBVar> m_x;
    GRBVar m_tau;

    void create_variables_x();
    void create_variable_tau();
    void create_objective();
public:
    explicit MasterProblem(const Instance& t_instance);
    void set_callback(Callback& t_cb);
};


#endif //CONVEX_ARO_FLP_MASTERPROBLEM_H
