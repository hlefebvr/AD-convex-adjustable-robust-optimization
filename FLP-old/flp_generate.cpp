//
// Created by henri on 10/07/22.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include "instance/flp_RandomInstance.h"

using namespace flp;

void generate_instance(const std::string& t_folder, unsigned int t_n_site, unsigned int t_n_clients, double t_ratio, unsigned int t_k) {
    RandomInstance instance(t_n_site, t_n_clients, t_ratio);

    std::stringstream filename;
    filename << "flp_instance_s" << t_n_site
             << "_c" << t_n_clients
             << "_r" << std::ceil(100. * t_ratio)
             << "__" << t_k
             << ".txt"
             ;

    std::ofstream file(t_folder + "/" + filename.str());
    if (!file.is_open()) { throw std::runtime_error("Could not open destination file."); }
    file << instance;
    file.close();

}

int main(int t_argc, const char** t_argv) {

    if (t_argc != 2) { throw std::runtime_error("Expected parameters: <DESTINATION_FOLDER>"); }
    const std::string path = t_argv[1];

    const std::vector<std::pair<unsigned int, unsigned int>> sizes = {
            { 10, 20 },
            { 10, 30 },
            { 15, 30 },
            { 15, 40 },
            { 20, 40 },
            { 20, 50 },
    };
    const std::vector<double> ratios = { 1.2, 1.4 };

    for (const auto& [n_site, n_clients] : sizes) {
        for (double ratio : ratios) {
            for (unsigned int k = 0 ; k < 5 ; k += 1) {
                generate_instance(path, n_site, n_clients, ratio, k);
            }
        }
    }

    return 0;
}