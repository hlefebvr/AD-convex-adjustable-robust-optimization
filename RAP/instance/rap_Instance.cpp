//
// Created by henri on 28/07/22.
//

#include "rap_Instance.h"
#include "../../cout_utils.h"

std::ostream& operator<<(std::ostream& t_os, const rap::Instance& t_instance) {
    t_os << t_instance.n_resources() << '\t' << t_instance.n_clients() << '\n';
    t_os << t_instance.m_c;
    t_os << t_instance.m_a;
    t_os << t_instance.m_d;
    t_os << t_instance.m_mu;
    return t_os;
}
