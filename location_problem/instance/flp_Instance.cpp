//
// Created by henri on 08/07/22.
//

#include "flp_Instance.h"
#include <ostream>
#include <algorithm>

std::ostream& operator<<(std::ostream& t_os, const std::vector<double>& t_vec) {
    std::for_each(t_vec.begin(), t_vec.end(), [&t_os](double elem){ t_os << elem << '\t'; });
    return t_os << '\n';
}

std::ostream& operator<<(std::ostream& t_os, const std::vector<std::vector<double>>& t_vec) {
    std::for_each(t_vec.begin(), t_vec.end(), [&t_os](const std::vector<double>& elem){ t_os << elem; });
    return t_os;
}

std::ostream& operator<<(std::ostream& t_os, const flp::Instance& t_instance) {
    t_os << t_instance.n_sites() << '\t' << t_instance.n_clients() << '\n';
    t_os << t_instance.m_f;
    t_os << t_instance.m_q;
    t_os << t_instance.m_d;
    t_os << t_instance.m_t;
    t_os << t_instance.m_a << '\t' << t_instance.m_b;
    return t_os;
}