//
// Created by henri on 28/07/22.
//

#include "rap_RobustCertificate.h"
#include "../instance/rap_Instance.h"

rap::RobustCertificate::RobustCertificate(const rap::Instance &t_instance)
        : m_pi(2),
          m_lambda(t_instance.n_resources()),
          m_theta(t_instance.n_resources()),
          m_xi(t_instance.n_clients()) {

    m_pi[0].resize(t_instance.n_clients());
    m_pi[1].resize(t_instance.n_resources());

}
