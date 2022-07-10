//
// Created by henri on 09/07/22.
//

#include "Model.h"

void Model::create_variables(std::vector<GRBVar> &t_dest, unsigned int t_n, double t_lb, double t_ub, char t_type) {
    t_dest.reserve(t_n);
    for (unsigned int i = 0 ; i < t_n ; i += 1) {
        t_dest.emplace_back(m_model.addVar(t_lb, t_ub, 0.0, t_type));
    }
}

void Model::create_variables(std::vector<std::vector<GRBVar>> &t_dest, unsigned int t_n, unsigned int t_m, double t_lb, double t_ub, char t_type) {
    t_dest.resize(t_n);
    for (unsigned int i = 0 ; i < t_n ; i += 1) {
        create_variables(t_dest[i], t_m, t_lb, t_ub, t_type);
    }
}

void Model::solve() {
    m_timer.start();
    m_model.optimize();
    m_timer.stop();
}

void Model::export_model(const std::string &t_model) {
    m_model.write(t_model);
}

Model::Model() : m_model(m_env) {
    m_model.set(GRB_IntParam_OutputFlag, 0);
}
