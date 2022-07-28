//
// Created by henri on 28/07/22.
//

#ifndef CONVEX_ARO_RAP_INSTANCEFROMFILE_H
#define CONVEX_ARO_RAP_INSTANCEFROMFILE_H

#include <string>
#include "rap_Instance.h"

namespace rap {
    class InstanceFromFile;
}

class rap::InstanceFromFile : public rap::Instance {
public:
    explicit InstanceFromFile(const std::string& t_path);
};


#endif //CONVEX_ARO_RAP_INSTANCEFROMFILE_H
