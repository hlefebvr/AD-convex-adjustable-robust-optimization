//
// Created by henri on 09/07/22.
//
#include "flp_Callback.h"
#include "../separation/flp_SeparationProblem.h"

flp::Callback::Callback(const Instance &t_instance, SeparationProblem &t_separation)
    : m_instance(t_instance), m_separation(t_separation) {}

flp::FirstStageProposition flp::Callback::get_proposition() {
    FirstStageProposition result(m_instance);

    for (unsigned int i = 0, n_sites = m_instance.n_sites() ; i < n_sites ; i += 1) {
        result.set_x_value(i, getSolution(m_x[i]));
    }

    result.set_tau_value(getSolution(m_tau));

    return result;
}

void flp::Callback::callback() {
    if (where != GRB_CB_MIPSOL) { return; }

    auto proposition = get_proposition();
    m_separation.update(proposition);
    m_separation.solve();
    auto certificate = m_separation.get_certificate();
    m_n_solved_separation += 1;

    if (certificate.objective_value() < -m_tolerance) { return; }

    add_cut(proposition, certificate);
    m_n_generated_scenarios += 1;
}
