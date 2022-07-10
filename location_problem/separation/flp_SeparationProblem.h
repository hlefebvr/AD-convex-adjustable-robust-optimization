//
// Created by henri on 09/07/22.
//

#ifndef CONVEX_ARO_FLP_SEPARATIONPROBLEM_H
#define CONVEX_ARO_FLP_SEPARATIONPROBLEM_H

#include "../../Model.h"
#include "flp_RobustCertificate.h"

namespace flp {
    class SeparationProblem;
    class Instance;
    class FirstStageProposition;
}

class flp::SeparationProblem : public Model {
    const Instance& m_instance;
    std::vector<std::vector<GRBVar>> m_pi;
    std::vector<GRBVar> m_theta;
    std::vector<GRBVar> m_v;
    std::vector<GRBVar> m_xi;
    std::vector<GRBVar> m_z;
    GRBVar m_mu;

    void create_variables_pi();
    void create_variables_theta();
    void create_variables_v();
    void create_variables_z();
    void create_variables_xi();
    void create_variable_mu();
    void create_constraint_simplex();
    void create_constraints_perspective_conjugate();
    void create_constraints_farkas();
    void create_objective_without_x();
    void create_constraint_budget();
    void create_linearization_constraints();
public:
    explicit SeparationProblem(const Instance& t_instance);
    void update(const FirstStageProposition& t_proposition);
    RobustCertificate get_certificate() const;
};


#endif //CONVEX_ARO_FLP_SEPARATIONPROBLEM_H
