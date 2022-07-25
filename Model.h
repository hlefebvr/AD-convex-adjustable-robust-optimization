//
// Created by henri on 09/07/22.
//

#ifndef CONVEX_ARO_MODEL_H
#define CONVEX_ARO_MODEL_H

#include <gurobi_c++.h>
#include "Timer.h"

class Model {
protected:
    GRBEnv m_env;
    GRBModel m_model;
    Timer m_timer;

    void create_variables(std::vector<GRBVar> &t_dest, unsigned int t_n, double t_lb, double t_ub, char t_type);
    void create_variables(std::vector<std::vector<GRBVar>> &t_dest, unsigned int t_n, unsigned int t_m, double t_lb, double t_ub, char t_type);
public:
    Model();
    void solve();
    void export_model(const std::string& t_model);
    [[nodiscard]] double objective_value() const { return m_model.get(GRB_DoubleAttr_ObjVal); }
    [[nodiscard]] const Timer& timer() const { return m_timer; }
    void set_time_limit(double t_time_timit);
};


#endif //CONVEX_ARO_MODEL_H
