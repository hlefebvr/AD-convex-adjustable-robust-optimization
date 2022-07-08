//
// Created by henri on 08/07/22.
//

#ifndef CONVEX_ARO_FLP_INSTANCEFROMFILE_H
#define CONVEX_ARO_FLP_INSTANCEFROMFILE_H

#include "flp_Instance.h"

namespace flp {
    class InstanceFromFile;
}

class flp::InstanceFromFile : public flp::Instance {
    static void read(std::ifstream& t_file, std::vector<double>& t_dest, unsigned int t_n);
    static void read(std::ifstream& t_file, std::vector<std::vector<double>>& t_dest, unsigned int t_n, unsigned int t_m);
public:
    explicit InstanceFromFile(const std::string& t_path);
};


#endif //CONVEX_ARO_FLP_INSTANCEFROMFILE_H
