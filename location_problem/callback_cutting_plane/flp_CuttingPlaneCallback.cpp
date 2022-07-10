//
// Created by henri on 09/07/22.
//

#include "flp_CuttingPlaneCallback.h"
#include "../separation/flp_RobustCertificate.h"
#include "../separation/flp_SeparationProblem.h"

flp::CuttingPlaneCallback::CuttingPlaneCallback(flp::SeparationProblem &t_separation)
    : Callback(t_separation) {}

void flp::CuttingPlaneCallback::add_cut(const flp::FirstStageProposition &t_proposition,
                                        const flp::RobustCertificate &t_certificate) {

    const Instance& instance = m_separation.instance();

    const unsigned int n_sites = instance.n_sites();
    const unsigned int n_clients = instance.n_clients();

    GRBLinExpr expr = -t_certificate.mu() * m_tau;

    for (unsigned int i = 0 ; i < n_sites ; ++i) {
        expr += -1. * instance.q(i) * t_certificate.pi(2, i) * m_x[i];
    }

    double offset = 0.;
    for (unsigned int i = 0 ; i < n_sites ; ++i) {
        offset += -1. * t_certificate.theta(i);
    }
    for (unsigned int j = 0 ; j < n_clients ; ++j) {
        offset += t_certificate.pi(0, j) * (instance.d(j) * instance.deviation() * t_certificate.xi(j) + instance.d(j));
    }

    addLazy(offset + expr <= 0);
}
