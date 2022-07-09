//
// Created by henri on 09/07/22.
//

#ifndef CONVEX_ARO_FLP_CUTTINGPLANECALLBACK_H
#define CONVEX_ARO_FLP_CUTTINGPLANECALLBACK_H

#include "flp_Callback.h"

namespace flp {
    class CuttingPlaneCallback;
}

class flp::CuttingPlaneCallback : public flp::Callback {
    void callback() override;
public:
    CuttingPlaneCallback(Instance& t_instance, SeparationProblem& t_separation);
};


#endif //CONVEX_ARO_FLP_CUTTINGPLANECALLBACK_H
