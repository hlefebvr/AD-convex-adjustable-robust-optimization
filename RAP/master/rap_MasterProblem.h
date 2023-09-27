//
// Created by henri on 28/07/22.
//

#ifndef CONVEX_ARO_RAP_MASTERPROBLEM_H
#define CONVEX_ARO_RAP_MASTERPROBLEM_H

#include "../../Model.h"
#include "rap_FirstStageProposition.h"

namespace rap {
    class MasterProblem;
    class Instance;
    class RobustCertificate;
}

class rap::MasterProblem : public Model {
    const Instance& m_instance;
    std::vector<GRBVar> m_x;

    unsigned int m_n_added_scenarios = 0;

    void create_variables_x();
    void create_objective();

    void create_constraints_v_definition(std::vector<std::vector<GRBVar>>& t_y, std::vector<GRBVar>& t_v);
    void create_constraints_capacity(std::vector<GRBVar>& t_v);
    void create_constraints_demand(std::vector<std::vector<GRBVar>>& t_y, const RobustCertificate& t_certificate);

    void create_feasibility_constraint();
public:
    explicit MasterProblem(const Instance& t_instance);

    void add_benders_cut(const RobustCertificate& t_certificate);
    void add_scenario_variables(const RobustCertificate& t_certificate);

    FirstStageProposition get_proposition();

    [[nodiscard]] const Instance& instance() const { return m_instance; }
    [[nodiscard]] unsigned int n_added_scenarios() const { return m_n_added_scenarios; }
    void increment_n_added_scenarios() { m_n_added_scenarios += 1; }
};


#endif //CONVEX_ARO_RAP_MASTERPROBLEM_H
