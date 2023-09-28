//
// Created by henri on 28.09.23.
//
#include <iostream>
#include <fstream>
#include "../instance/Instance.h"

int main(int t_argc, const char** t_argv) {

    if (t_argc != 5) {
        throw std::invalid_argument("Arguments: <destination_folder> <n_servers> <n_clients> <n_instances>");
    }

    const std::string destination_folder = t_argv[1];
    const auto n_servers = std::stoul(t_argv[2]);
    const auto n_clients = std::stoul(t_argv[3]);
    const auto n_instances = std::stoul(t_argv[4]);

    for (unsigned int index = 0 ; index < n_instances ; ++index) {

        const std::string path_to_file = destination_folder
                                         + "/instance_S" + std::to_string(n_servers)
                                         + "_C" + std::to_string(n_clients)
                                         + "__" + std::to_string(index)
                                         + ".txt";

        std::ofstream file(path_to_file);

        if (!file.is_open()) {
            throw std::runtime_error("Could not open destination file: " + path_to_file + ".");
        }

        const auto instance = RAP::generate_instance(n_servers, n_clients);

        file << instance;

        file.close();

    }

    return 0;
}
