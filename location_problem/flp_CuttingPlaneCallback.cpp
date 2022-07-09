//
// Created by henri on 09/07/22.
//

#include "flp_CuttingPlaneCallback.h"
#include "flp_SeparationProblem.h"
#include "flp_Instance.h"

flp::CuttingPlaneCallback::CuttingPlaneCallback(flp::Instance& t_instance, flp::SeparationProblem &t_separation)
    : Callback(t_instance, t_separation) {}

void flp::CuttingPlaneCallback::callback() {
    if (where != GRB_CB_MIPSOL) { return; }

    auto proposition = get_proposition();
    m_separation.update(proposition);
    m_separation.solve();
    auto certificate = m_separation.get_certificate();

    if (certificate.objective_value() < -m_tolerance) { return; }

    const unsigned int n_sites = m_instance.n_sites();
    const unsigned int n_clients = m_instance.n_clients();

    GRBLinExpr expr = -certificate.mu() * m_tau;

    for (unsigned int i = 0 ; i < n_sites ; ++i) {
        expr += -1. * m_instance.q(i) * certificate.pi(2, i) * m_x[i];
    }

    double offset = 0.;
    for (unsigned int i = 0 ; i < n_sites ; ++i) {
        offset += -1. * certificate.theta(i);
    }
    for (unsigned int j = 0 ; j < n_clients ; ++j) {
        offset += certificate.pi(0, j) * (m_instance.d(j) * m_instance.deviation() * certificate.xi(j) + m_instance.d(j));
    }

    addLazy(offset + expr <= 0);
}
