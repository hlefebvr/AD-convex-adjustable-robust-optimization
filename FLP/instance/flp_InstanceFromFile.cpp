//
// Created by henri on 08/07/22.
//

#include "flp_InstanceFromFile.h"
#include "../../cin_utils.h"


flp::InstanceFromFile::InstanceFromFile(const std::string &t_path) {

    std::ifstream file(t_path);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file " + t_path + ". File not found.");
    }

    file >> m_n_sites >> m_n_clients;
    read_from_file(file, m_f, m_n_sites);
    read_from_file(file, m_q, m_n_sites);
    read_from_file(file, m_d, m_n_clients);
    read_from_file(file, m_t, m_n_sites, m_n_clients);
    file >> m_a >> m_b;

    file.close();
}
