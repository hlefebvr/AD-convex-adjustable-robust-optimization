//
// Created by henri on 28/07/22.
//

#include "rap_InstanceFromFile.h"
#include "../../cin_utils.h"

rap::InstanceFromFile::InstanceFromFile(const std::string &t_path) {
    std::ifstream file(t_path);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file " + t_path + ". File not found.");
    }

    file >> m_n_resources >> m_n_clients;
    read_from_file(file, m_c, m_n_resources);
    read_from_file(file, m_a, m_n_resources);
    read_from_file(file, m_d, m_n_clients);
    read_from_file(file, m_mu, m_n_resources, m_n_clients);

    file.close();
}
