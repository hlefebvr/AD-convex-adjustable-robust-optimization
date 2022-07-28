//
// Created by henri on 28/07/22.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include "instance/rap_RandomInstance.h"

using namespace rap;

void generate_instance(const std::string& t_folder, unsigned int t_n_resources, unsigned int t_n_clients, unsigned int t_k) {
    RandomInstance instance(t_n_resources, t_n_clients);

    std::stringstream filename;
    filename << "rap_instance_r" << t_n_resources
             << "_c" << t_n_clients
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

    for (const auto& [n_site, n_clients] : sizes) {
        for (unsigned int k = 0 ; k < 5 ; k += 1) {
            generate_instance(path, n_site, n_clients, k);
        }
    }

    return 0;
}