//
// Created by henri on 28/07/22.
//

#ifndef CONVEX_ARO_CIN_UTILS_H
#define CONVEX_ARO_CIN_UTILS_H

#include <fstream>
#include <vector>

void read_from_file(std::ifstream &t_file, std::vector<double> &t_dest, unsigned int t_n) {

    t_dest.resize(t_n);
    for (unsigned int i = 0 ; i < t_n ; ++i) {
        t_file >> t_dest[i];
    }

}

void read_from_file(std::ifstream &t_file, std::vector<std::vector<double>> &t_dest, unsigned int t_m, unsigned int t_n) {

    t_dest.resize(t_m);
    for (unsigned int i = 0 ; i < t_m ; ++i) {
        read_from_file(t_file, t_dest[i], t_n);
    }

}


#endif //CONVEX_ARO_CIN_UTILS_H
