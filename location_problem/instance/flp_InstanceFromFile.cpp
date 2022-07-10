//
// Created by henri on 08/07/22.
//

#include "flp_InstanceFromFile.h"
#include <fstream>

void flp::InstanceFromFile::read(std::ifstream &t_file, std::vector<double> &t_dest, unsigned int t_n) {

    t_dest.resize(t_n);
    for (unsigned int i = 0 ; i < t_n ; ++i) {
        t_file >> t_dest[i];
    }

}

void flp::InstanceFromFile::read(std::ifstream &t_file, std::vector<std::vector<double>> &t_dest, unsigned int t_m, unsigned int t_n) {

    t_dest.resize(t_m);
    for (unsigned int i = 0 ; i < t_m ; ++i) {
        read(t_file, t_dest[i], t_n);
    }

}


flp::InstanceFromFile::InstanceFromFile(const std::string &t_path) {

    std::ifstream file(t_path);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file " + t_path + ". File not found.");
    }

    file >> m_n_sites >> m_n_clients;
    read(file, m_f, m_n_sites);
    read(file, m_q, m_n_sites);
    read(file, m_d, m_n_clients);
    read(file, m_t, m_n_sites, m_n_clients);
    file >> m_a >> m_b;

}
