//
// Created by henri on 09/07/22.
//

#include "flp_ScenarioGenerationCallback.h"

flp::ScenarioGenerationCallback::ScenarioGenerationCallback(flp::Instance &t_instance, flp::SeparationProblem &t_separation)
    : Callback(t_instance, t_separation) {}

void flp::ScenarioGenerationCallback::add_cut(const flp::FirstStageProposition &t_proposition,
                                              const flp::RobustCertificate &t_certificate) {

    const unsigned int n_sites = m_instance.n_sites();
    const unsigned int n_clients = m_instance.n_clients();

}
