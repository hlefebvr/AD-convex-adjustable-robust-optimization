//
// Created by henri on 09/07/22.
//

#ifndef CONVEX_ARO_MODEL_H
#define CONVEX_ARO_MODEL_H

#include <gurobi_c++.h>

class Model {
protected:
    GRBEnv m_env;
    GRBModel m_model;

    void create_variables(std::vector<GRBVar> &t_dest, unsigned int t_n, double t_lb, double t_ub, char t_type);
public:
    Model();
    void solve();
    void export_model(const std::string& t_model);
    double objective_value() const { return m_model.get(GRB_DoubleAttr_ObjVal); }
};


#endif //CONVEX_ARO_MODEL_H
