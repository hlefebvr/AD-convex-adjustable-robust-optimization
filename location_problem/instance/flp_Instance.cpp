//
// Created by henri on 08/07/22.
//

#include "flp_Instance.h"
#include "../../cout_utils.h"

std::ostream& operator<<(std::ostream& t_os, const flp::Instance& t_instance) {
    t_os << t_instance.n_sites() << '\t' << t_instance.n_clients() << '\n';
    t_os << t_instance.m_f;
    t_os << t_instance.m_q;
    t_os << t_instance.m_d;
    t_os << t_instance.m_t;
    t_os << t_instance.m_a << '\t' << t_instance.m_b;
    return t_os;
}