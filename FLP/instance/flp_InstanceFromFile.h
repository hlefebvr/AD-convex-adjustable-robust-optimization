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
public:
    explicit InstanceFromFile(const std::string& t_path);
};


#endif //CONVEX_ARO_FLP_INSTANCEFROMFILE_H
