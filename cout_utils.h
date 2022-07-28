//
// Created by henri on 28/07/22.
//

#ifndef CONVEX_ARO_COUT_UTILS_H
#define CONVEX_ARO_COUT_UTILS_H

#include <iostream>
#include <algorithm>

std::ostream& operator<<(std::ostream& t_os, const std::vector<double>& t_vec) {
    std::for_each(t_vec.begin(), t_vec.end(), [&t_os](double elem){ t_os << elem << '\t'; });
    return t_os << '\n';
}

std::ostream& operator<<(std::ostream& t_os, const std::vector<std::vector<double>>& t_vec) {
    std::for_each(t_vec.begin(), t_vec.end(), [&t_os](const std::vector<double>& elem){ t_os << elem; });
    return t_os;
}

#endif //CONVEX_ARO_COUT_UTILS_H
